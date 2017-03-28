/**
 * @par Copyright 2017 Wuhan OS-Easy Cloud Computing Co. Ltd.
 * @file cmds.c
 * @brief fcadm command.
 * @author LastRitter
 * @version 1.0.0
 * @date 2017-01-20
 */
#include "fcadm.h"


#define FC_SHOW_SEQ_NUM		(1 << 4)
#define FC_SHOW_LINE_NUM	(1 << 7)


FC_LOCAL_INLINE void fc_cmd_show_one(const uint8_t*  cache_map, int64_t i, int64_t j)
{
	if(cache_map[(i * FC_SHOW_LINE_NUM) + j] == 0)
	{
		FC_OUT(" ");
	}
	else
	{
		FC_OUT("*");
	}
}


FC_LOCAL_INLINE void fc_cmd_show_line(const fc_params_t* params, const uint8_t* cache_map, int64_t i, int64_t num)
{
	int64_t j;
	
	FC_OUT("[%010lX] ", i * FC_SHOW_LINE_NUM * params->page_size);
	
	for(j=0; j<num; j++)
	{
		fc_cmd_show_one(cache_map, i, j);
		
		if((j & (FC_SHOW_SEQ_NUM - 1)) == (FC_SHOW_SEQ_NUM - 1))
		{
			FC_OUT(" ");
		}
	}
	
	FC_OUT("\n");
}


FC_LOCAL int fc_cmd_show(fc_params_t* const params)
{
	uint8_t* const cache_map = malloc(params->page_num);
	if(cache_map == NULL)
	{
		FC_ERR("alloc memory failed.\n");
		return -1;
	}
	
	if(mincore(params->file_mem, params->file_size, cache_map) != 0)
	{
		FC_ERR("mmap file failed.\n");
		free(cache_map);
		return -1;
	}
	
	int64_t compare_line = 0;
	int64_t i, j;
	
	FC_OUT("file cache map \n");
	fc_cmd_show_line(params, cache_map, 0, FC_SHOW_LINE_NUM);
	for(i=1; i<params->page_num/FC_SHOW_LINE_NUM; i++)
	{
		for(j=0; j<FC_SHOW_LINE_NUM; j++)
		{
			if(cache_map[(i * FC_SHOW_LINE_NUM) + j] != cache_map[(compare_line * FC_SHOW_LINE_NUM) + j])
			{
				break;
			}
		}
		
		if(j != FC_SHOW_LINE_NUM)
		{
			fc_cmd_show_line(params, cache_map, i, FC_SHOW_LINE_NUM);
			compare_line = i;
			continue;
		}
		
		if(i == (compare_line + 1))
		{
			FC_OUT(" .......... \n\n");
		}
	}
	
	if(params->page_num % FC_SHOW_LINE_NUM)
	{
		fc_cmd_show_line(params, cache_map, i, params->page_num % FC_SHOW_LINE_NUM);
	}
	
	int64_t cached_num = 0;
	for(i=0; i<params->page_num; i++)
	{
		if(cache_map[i] != 0)
		{
			cached_num ++;
		}
	}
	
	FC_OUT("total cache num %ld, size 0x%08lX.\n", cached_num, cached_num * params->page_size);
	
	free(cache_map);
	return 0;
}


FC_LOCAL int fc_cmd_load(fc_params_t* const params)
{
	int aa_index;
	
	for(aa_index=0; aa_index<params->aa_num; aa_index++)
	{
		const size_t off = params->aa_map[aa_index].off;
		const size_t len = params->aa_map[aa_index].len;
		
		if(posix_fadvise(params->fd, off, len, POSIX_FADV_WILLNEED) < 0)
		{
			FC_ERR("load cache failed, %s\n", strerror(errno));
			return -1;
		}
	
		size_t i;
		for(i=off; i<(off + len); i+=params->page_size)
		{
			char tmp = params->file_mem[i];
		}
	}
	
	return 0;
}


FC_LOCAL int fc_cmd_drop(fc_params_t* const params)
{
	int aa_index;
	
	for(aa_index=0; aa_index<params->aa_num; aa_index++)
	{
		const size_t off = params->aa_map[aa_index].off;
		const size_t len = params->aa_map[aa_index].len;
		
		if(posix_fadvise(params->fd, off, len, POSIX_FADV_DONTNEED) < 0)
		{
			FC_ERR("drop cache failed, %s\n", strerror(errno));
			return -1;
		}
	}
	
	return 0;
}


FC_LOCAL int fc_cmd_lock(fc_params_t* const params)
{
	int ret = fc_cmd_load(params);
	if(ret != 0)
	{
		return ret;
	}
	
	int aa_index;
	for(aa_index=0; aa_index<params->aa_num; aa_index++)
	{
		const size_t off = params->aa_map[aa_index].off;
		const size_t len = params->aa_map[aa_index].len;
		
		ret = mlock(&params->file_mem[off], len);
		if(ret != 0)
		{
			return ret;
		}
	}
	
	while(!params->is_break)
	{
		FC_INF("press ctrl + c to exit.\n");
		sleep(3);
	}
	
	return 0;
}


FC_LOCAL int fc_cmd_unlock(fc_params_t* const params)
{
	int ret = 0;
	int aa_index;
	
	for(aa_index=0; aa_index<params->aa_num; aa_index++)
	{
		const size_t off = params->aa_map[aa_index].off;
		const size_t len = params->aa_map[aa_index].len;
		
		ret = munlock(&params->file_mem[off], len);
		if(ret != 0)
		{
			return ret;
		}
	}
	
	return fc_cmd_drop(params);
}


FC_LOCAL int fc_mem_lock_test(fc_params_t* const params, int alloc_size)
{
	char* memory = malloc(alloc_size);
	if(mlock(memory, alloc_size) == -1)
	{
		perror("mlock");
		return (-1);
	}
	
	size_t i;
	size_t page_size = getpagesize();
	for(i=0; i<alloc_size; i+=page_size)
	{
		FC_ERR("i=%zd\n",i);
		memory[i] = 0;
	}
	
	while(!params->is_break)
	{
		FC_INF("press ctrl + c to exit.\n");
		sleep(1);
	}
	
	if(munlock(memory, alloc_size) == -1)
	{
		perror("munlock");
		return (-1);
	}
	return 0;
}


FC_LOCAL int fc_cmd_test(fc_params_t* const params)
{
	return fc_mem_lock_test(params, 32 << 20);
}


FC_LOCAL const dmd_cmd_t dmd_cmds[FC_CMD_MAX] = 
{
	{"show",	FC_CMD_SHOW,	fc_cmd_show, }, 
	{"load",	FC_CMD_LOAD,	fc_cmd_load, }, 
	{"drop",	FC_CMD_DROP,	fc_cmd_drop, }, 
	{"lock",	FC_CMD_LOCK,	fc_cmd_lock, }, 
	{"unlock",	FC_CMD_UNLOCK,	fc_cmd_unlock, }, 
	{"test",	FC_CMD_TEST,	fc_cmd_test, }, 
	{"help",	FC_CMD_HELP,	NULL }, 
};


int dmd_cmd_run(fc_params_t* const params)
{
	if(params->cmd > FC_CMD_MAX)
	{
		FC_ERR("error command %d.\n", params->cmd);
		return -1;
	}

	return dmd_cmds[params->cmd].func(params);
}


