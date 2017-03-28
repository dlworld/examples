/**
 * @par Copyright 2017 Wuhan OS-Easy Cloud Computing Co. Ltd.
 * @file main.c
 * @brief fcadm main source code.
 * @author LastRitter
 * @version 1.0.0
 * @date 2017-01-19
 */
#include "fcadm.h"



FC_LOCAL fc_params_t fc_params = 
{
	.is_break	= 0, 
	
	.cmd		= -1, 
	
	.path		= { 0 }, 
	.fd			= -1, 
	
	.file_mem	= NULL, 
	
	.file_size	= 0, 
	.page_size	= 0, 
	
	.aa_num		= 0, 
	.aa_map		= { 0 }, 
};


FC_LOCAL void fc_sigint_cb(int signum)
{
	fc_params.is_break = 1;
}


FC_LOCAL int fc_file_map(fc_params_t* const params)
{
	if(access(params->path, F_OK) != 0)
	{
		FC_ERR("file %s is not exist.\n", params->path);
		return -1;
	}
	
	params->fd = open(params->path, O_RDONLY, 0);
	if(params->fd <= 0)
	{
		FC_ERR("open %s failed.\n", params->path);
		return -1;
	}
	
//	lseek(fd, 0L, SEEK_END);
//	size_t file_size = ftell(fd);
	struct stat statbuff;
	if(stat(params->path, &statbuff) < 0)
	{
		FC_ERR("get %s stat info failed.\n", params->path);
		close(params->fd);
		return -1;
	}
	params->file_size = FC_ALIGN_UP(statbuff.st_size, params->page_size);
	params->page_num = params->file_size / params->page_size;
	
	params->file_mem = mmap(NULL, params->file_size, PROT_READ, MAP_SHARED, params->fd, 0);
	if(params->file_mem == (void*)-1)
	{
		FC_ERR("mmap failed, %s.\n", params->path);
		close(params->fd);
		return -1;
	}
	
	return 0;
}


FC_LOCAL int fc_file_unmap(fc_params_t* const params)
{
	munmap(params->file_mem, params->file_size);
	close(params->fd);
	
	return 0;
}


FC_LOCAL int fc_aa_fixup(fc_params_t* const params)
{
	if(params->aa_num == 0)
	{
		params->aa_num = 1;
		params->aa_map[0].off = 0;
		params->aa_map[0].len = params->file_size < (256 * FC_SIZE_1M) ? params->file_size : (256 * FC_SIZE_1M);
		
		return 0;
	}
	
	int aa_index;
	size_t total_len = 0;
	for(aa_index=0; aa_index<params->aa_num; aa_index++)
	{
		size_t off = FC_ALIGN_DOWN(params->aa_map[aa_index].off, params->page_size);
		size_t len = FC_ALIGN_DOWN(params->aa_map[aa_index].len, params->page_size);
		
		if(len == FC_ALIGN_DOWN(-1, params->page_size))
		{
			len = FC_ALIGN_DOWN(params->file_size - off, params->page_size);
			FC_OUT("fixup address area [%05d] to 0x%016lX:0x%016lX.\n", aa_index, off, len);
		}
		
		if(((off + len) > params->file_size))
		{
			FC_ERR("address area 0x%016lX:0x%016lX is out of file size.\n", off, len);
			return -1;
		}
		
		params->aa_map[aa_index].off = off;
		params->aa_map[aa_index].len = len;
		
		total_len += len;
	}
	
	struct sysinfo si;
	sysinfo(&si);
//	printf("system total ram -  %016lX.\n", si.totalram);
//	printf("system available -  %016lX.\n", si.freeram);
//	printf("file size        -  %016lX.\n", params->file_size);
	
	size_t max_len = si.totalram;
	if(params->cmd == FC_CMD_LOCK)
	{
		max_len = si.freeram * 95 / 100;
	}
	
/*	if(total_len > max_len)
	{
		FC_ERR("too much memory for address area, total_len %016lX, available %016lX, file_size %016lX.\n", total_len, si.freeram, params->file_size);
		return -1;
	}*/
	
	return 0;
}


int main(int argc, char* argv[])
{
	if(signal(SIGINT, fc_sigint_cb) == SIG_ERR)
	{
		FC_ERR("install ctrl+c signal failed.\n");
		return -1;
	}
	
	int ret = fc_params_parse(&fc_params, argc, argv);
	if(ret != 1)
	{
		return ret;
	}
	
	if(fc_file_map(&fc_params) != 0)
	{
		return -1;
	}
	
	if(fc_aa_fixup(&fc_params) != 0)
	{
		fc_file_unmap(&fc_params);
		return -1;
	}
	
	FC_OUT("fixup address area : \n");
	int i;
	for(i=0; i<fc_params.aa_num; i++)
	{
		FC_OUT("[%05d] 0x%016lX:0x%016lX\n", i, fc_params.aa_map[i].off, fc_params.aa_map[i].len);
	}
	
	ret = dmd_cmd_run(&fc_params);
	fc_file_unmap(&fc_params);
	return ret;
}


