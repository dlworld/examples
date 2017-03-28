/**
 * @par Copyright 2017 Wuhan OS-Easy Cloud Computing Co. Ltd.
 * @file params.c
 * @brief fcadm parameters parse.
 * @author LastRitter
 * @version 1.0.0
 * @date 2017-01-20
 */
#include "fcadm.h"



FC_LOCAL const struct option fc_opts[FC_CMD_MAX + 1] = 
{
	{"show",	required_argument,	NULL,	DMD_CMD_CHAR(FC_CMD_SHOW), }, 
	{"load",	required_argument,	NULL,	DMD_CMD_CHAR(FC_CMD_LOAD), }, 
	{"drop",	required_argument,	NULL,	DMD_CMD_CHAR(FC_CMD_DROP), }, 
	{"lock",	required_argument,	NULL,	DMD_CMD_CHAR(FC_CMD_LOCK), }, 
	{"unlock",	required_argument,	NULL,	DMD_CMD_CHAR(FC_CMD_UNLOCK), }, 
	{"test",	no_argument,		NULL,	DMD_CMD_CHAR(FC_CMD_TEST), }, 
	{"help",	no_argument,		NULL,	DMD_CMD_CHAR(FC_CMD_HELP), }, 
	{ 0, 0, 0, 0 }, 
};


FC_LOCAL int fc_params_help(int ret)
{
	FC_OUT("fcadm usage \n" 
		"  --show   <path> [-a \"address area\"]\n"
		"  --load   <path> [-a \"address area\"]\n" 
		"  --drop   <path> [-a \"address area\"]\n" 
		"  --lock   <path> [-a \"address area\"]\n" 
		"  --unload <path> [-a \"address area\"]\n"
		"  --test   \n"
		"  --help   \n");
	
	return ret;
}


FC_LOCAL int fc_params_parse_area(fc_params_t* const params, const char* aa_str)
{
	char str[8192];
	
	if(strlen(aa_str) > (sizeof(str) - 1))
	{
		FC_ERR("address area string is too long.\n");
		return -1;
	}
	strcpy(str, aa_str);
	
	char* paa = strtok(str, ";");
	while(paa != NULL)
	{
		char* plen = strchr(paa, ':');
		if(plen == NULL)
		{
			FC_ERR("invalid address area string \"%s\".", aa_str);
			return -1;
		}
		*plen++ = 0;
		
		if((paa + 1) == plen)
		{
			params->aa_map[params->aa_num].off = 0;
		}
		else
		{
			if(fc_str_to_size(paa, &params->aa_map[params->aa_num].off) != FC_OK)
			{
				FC_ERR("invalid address area string \"%s\".", aa_str);
				return -1;
			}
		}
		
		if(fc_str_to_size(plen, &params->aa_map[params->aa_num].len) != FC_OK)
		{
			FC_ERR("invalid address area string \"%s\".", aa_str);
			return -1;
		}
		
		if(++params->aa_num >= FC_NUM_OF(params->aa_map))
		{
			FC_ERR("too many address area \"%s\".", aa_str);
			return -1;
		}
		
		paa = strtok(NULL, ";");
	}
	
/*	FC_OUT("view address area : \n");
	int i;
	for(i=0; i<params->aa_num; i++)
	{
		FC_OUT("[%05d] 0x%016lX:0x%016lX\n", i, params->aa_map[i].off, params->aa_map[i].len);
	}*/
	
	return 0;
}


int fc_params_parse(fc_params_t* const params, int argc, char* argv[])
{
	if(argc < 2)
	{
		return fc_params_help(-1);
	}
	
	int i;
	for(i=0; i<(FC_NUM_OF(fc_opts) - 1); i++)
	{
		if(strcmp(argv[1], fc_opts[i].name) == 0)
		{
			params->cmd = DMD_CMD_CODE(fc_opts[i].val);
			
			if((fc_opts[i].has_arg == no_argument) || (argc < 3))
			{
				break;
			}
			
			if((strlen(argv[2]) < 1) || (strlen(argv[2]) > (sizeof(params->path) - 1)))
			{
				FC_ERR("path is invalid or too long.\n");
				return -1;
			}
			strcpy(params->path, argv[2]);
			break;
		}
	}
	
	char opts[32];
	for(i=0; i<(FC_NUM_OF(fc_opts) - 1); i++)
	{
		opts[i] = fc_opts[i].val;
	}
	opts[i] = 0;
	strcat(opts, "a:h");
	
	int index = 0;
	int opt = 0;
	while((opt = getopt_long(argc, argv, opts, fc_opts, &index)) != -1)
	{
		if((opt >= DMD_CMD_CHAR_BASE) && (opt < DMD_CMD_CHAR_MAX))
		{
			params->cmd = DMD_CMD_CODE(opt);
			
			if((fc_opts[index].has_arg == no_argument) || (optarg == NULL))
			{
				continue;
			}
			
			if((strlen(optarg) < 1) || (strlen(optarg) > (sizeof(params->path) - 1)))
			{
				FC_ERR("path is invalid or too long.\n");
				return -1;
			}
			strcpy(params->path, optarg);
			continue;
		}
		
		switch(opt)
		{
			case 'a' : 
			{
				if(fc_params_parse_area(params, optarg) != 0)
				{
					return -1;
				}
				break;
			}
			
			case 'h' : return fc_params_help(0);
			
			default : return fc_params_help(-1);
		}
	}
	
	if((params->cmd < 0) || (params->cmd >= (FC_NUM_OF(fc_opts) - 1)))
	{
		return fc_params_help(-1);
	}
	
	if(params->cmd == FC_CMD_HELP)
	{
		return fc_params_help(0);
	}
	
	params->page_size = getpagesize();	// sysconf(_SC_PAGESIZE)
	
	return 1;
}


