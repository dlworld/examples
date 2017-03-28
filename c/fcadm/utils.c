/**
 * @par Copyright 2017 Wuhan OS-Easy Cloud Computing Co. Ltd.
 * @file utils.c
 * @brief fcadm utils function.
 * @author LastRitter
 * @version 1.0.0
 * @date 2017-01-19
 */
#include "fcadm.h"



/**
 * @brief fc_str_to_size, parse size value frome string.
 */
int fc_str_to_size(const char* str, uint64_t* val)
{
	uint64_t multiple = 1;
	char* endp;
	
	if(FC_UNLIKELY(str == NULL) || FC_UNLIKELY(val == NULL))
	{
		FC_ERR("invalid parameters, str=%p, val=%p.\n", str, val);
		return -FC_INVALID_PARAM;
	}
	
	if(strlen(str) > 1)
	{
		switch(str[strlen(str) - 1])
		{
			case 'k' : 
			case 'K' : 
				multiple = FC_SIZE_1K;
				break;
			
			case 'm' : 
			case 'M' : 
				multiple = FC_SIZE_1M;
				break;
			
			case 'g' : 
			case 'G' : 
				multiple = FC_SIZE_1G;
				break;
			
			case 't' : 
			case 'T' : 
				multiple = FC_SIZE_1T;
				break;
			
			default : 
				break;
		}
	}
	
#if defined(__KERNEL__) && defined(__linux__)
	*val = simple_strtoul(str, &endp, 0);
#else
	*val = strtoul(str, &endp, 0);
#endif
	
	if(FC_UNLIKELY(endp == str) || FC_UNLIKELY(endp < &str[strlen(str) - 1]))
	{
		FC_ERR("invalid parameters \"%s\".\n", str);
		return FC_FAILED;
	}
	
	*val *= multiple;
	return FC_OK;
}


/**
 * @brief fc_str_to_size, parse size value frome string.
 */
const char* fc_size_to_str(uint64_t val, char* str, int len)
{
	if(FC_UNLIKELY(str == NULL) || FC_UNLIKELY(len < 1))
	{
		return NULL;
	}
	memset(str, 0, len);
	
	if((val == 0) || (val & (~((1UL << 10) - 1))) != val)
	{
		snprintf(str, len, FC_UINT64_STR_FORMAT, val);
		return str;
	}
	
	if((val & (~((1UL << 20) - 1))) != val)
	{
		snprintf(str, len, FC_UINT64_STR_FORMAT "K", val >> 10);
		return str;
	}
	
	if((val & (~((1UL << 30) - 1))) != val)
	{
		snprintf(str, len, FC_UINT64_STR_FORMAT "M", val >> 20);
		return str;
	}
	
	if((val & (~((1UL << 40) - 1))) != val)
	{
		snprintf(str, len, FC_UINT64_STR_FORMAT "G", val >> 30);
		return str;
	}
	
	snprintf(str, len, FC_UINT64_STR_FORMAT "T", val >> 40);
	return str;
}


const char* fc_char_to_str(char* ch, int num, char* str, int len)
{
	int offset = 0;
	int i = 0;
	
	if(FC_UNLIKELY(ch == NULL) || FC_UNLIKELY(num < 1) || FC_UNLIKELY(str == NULL) || FC_UNLIKELY(len < 1))
	{
		FC_ERR("invalid parameters, ch=%p, num=%d, str=%p, len=%d.\n", ch, num, str, len);
		return str;
	}
	
	for(i=0; i< num; i++)
	{
		FC_SNPRINT_BREAK(str, len, offset, "%c", ch[i]);
	}
	
	return str;
}


#if !defined(__KERNEL__) || !defined(__linux__)

int fc_epoll_noblock_set(int fd)
{
	int old_opt = fcntl(fd, F_GETFL);
	int new_opt = old_opt | O_NONBLOCK;
	
	if(old_opt < 0)
	{
		FC_ERR("get old option failed, errno=%d.\n", errno);
		return old_opt;
	}
	
	new_opt = fcntl(fd, F_SETFL, new_opt);
	if(new_opt < 0)
	{
		FC_ERR("set new option failed, errno=%d.\n", errno);
		return new_opt;
	}
	
	return old_opt;
}


int fc_epoll_fd_add(int epollfd, int fd, int oneshot)
{
	struct epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET;
	
	if(oneshot == FC_TRUE)
	{
		event.events |= EPOLLONESHOT;
	}
	
	if(epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event) != 0)
	{
		FC_ERR("add fd failed, errno=%d.\n", errno);
		return FC_FAILED;
	}
	
	if(fc_epoll_noblock_set(fd) < 0)
	{
		FC_ERR("set noblock failed.\n");
		return FC_FAILED;
	}
	
	return FC_OK;
}


int fc_epoll_oneshot_reset(int epollfd, int fd)
{
	struct epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
	
	return epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}


int fc_hostname_get(char* hostname, int len)
{
	if(FC_UNLIKELY(hostname == NULL))
	{
		FC_ERR("invalid parameters, hostname=%p.\n", hostname);
		return -FC_INVALID_PARAM;
	}
	
	FILE* fp = popen("cat /proc/sys/kernel/hostname", "r");
	if(fp == NULL)
	{
		pclose(fp);
		return FC_FAILED;
	}
	
	if(fgets(hostname, len, fp) != hostname)
	{
		pclose(fp);
		return FC_FAILED;
	}
	
	if(strlen(hostname) > (len - 1))
	{
		hostname[len - 1] = 0;
	}
	else
	{
		hostname[strlen(hostname) - 1] = 0;
	}
	
	pclose(fp);
	return FC_OK;
}

#endif /* !defined(__KERNEL__) || !defined(__linux__) */


FC_LOCAL const uint32_t fc_crc32_table[256] = 
{
	0x00000000, 0x04c10db7, 0x09821b6e, 0x0d4316d9,
	0x130436dc, 0x17c53b6b, 0x1a862db2, 0x1e472005,
	0x26086db8, 0x22c9600f, 0x2f8a76d6, 0x2b4b7b61,
	0x350c5b64, 0x31cd56d3, 0x3c8e400a, 0x384f4dbd,
	0x4c10db70, 0x48d1d6c7, 0x4592c01e, 0x4153cda9,
	0x5f14edac, 0x5bd5e01b, 0x5696f6c2, 0x5257fb75,
	0x6a18b6c8, 0x6ed9bb7f, 0x639aada6, 0x675ba011,
	0x791c8014, 0x7ddd8da3, 0x709e9b7a, 0x745f96cd,
	0x9821b6e0, 0x9ce0bb57, 0x91a3ad8e, 0x9562a039,
	0x8b25803c, 0x8fe48d8b, 0x82a79b52, 0x866696e5,
	0xbe29db58, 0xbae8d6ef, 0xb7abc036, 0xb36acd81,
	0xad2ded84, 0xa9ece033, 0xa4aff6ea, 0xa06efb5d,
	0xd4316d90, 0xd0f06027, 0xddb376fe, 0xd9727b49,
	0xc7355b4c, 0xc3f456fb, 0xceb74022, 0xca764d95,
	0xf2390028, 0xf6f80d9f, 0xfbbb1b46, 0xff7a16f1,
	0xe13d36f4, 0xe5fc3b43, 0xe8bf2d9a, 0xec7e202d,
	0x34826077, 0x30436dc0, 0x3d007b19, 0x39c176ae,
	0x278656ab, 0x23475b1c, 0x2e044dc5, 0x2ac54072,
	0x128a0dcf, 0x164b0078, 0x1b0816a1, 0x1fc91b16,
	0x018e3b13, 0x054f36a4, 0x080c207d, 0x0ccd2dca,
	0x7892bb07, 0x7c53b6b0, 0x7110a069, 0x75d1adde,
	0x6b968ddb, 0x6f57806c, 0x621496b5, 0x66d59b02,
	0x5e9ad6bf, 0x5a5bdb08, 0x5718cdd1, 0x53d9c066,
	0x4d9ee063, 0x495fedd4, 0x441cfb0d, 0x40ddf6ba,
	0xaca3d697, 0xa862db20, 0xa521cdf9, 0xa1e0c04e,
	0xbfa7e04b, 0xbb66edfc, 0xb625fb25, 0xb2e4f692,
	0x8aabbb2f, 0x8e6ab698, 0x8329a041, 0x87e8adf6,
	0x99af8df3, 0x9d6e8044, 0x902d969d, 0x94ec9b2a,
	0xe0b30de7, 0xe4720050, 0xe9311689, 0xedf01b3e,
	0xf3b73b3b, 0xf776368c, 0xfa352055, 0xfef42de2,
	0xc6bb605f, 0xc27a6de8, 0xcf397b31, 0xcbf87686,
	0xd5bf5683, 0xd17e5b34, 0xdc3d4ded, 0xd8fc405a,
	0x6904c0ee, 0x6dc5cd59, 0x6086db80, 0x6447d637,
	0x7a00f632, 0x7ec1fb85, 0x7382ed5c, 0x7743e0eb,
	0x4f0cad56, 0x4bcda0e1, 0x468eb638, 0x424fbb8f,
	0x5c089b8a, 0x58c9963d, 0x558a80e4, 0x514b8d53,
	0x25141b9e, 0x21d51629, 0x2c9600f0, 0x28570d47,
	0x36102d42, 0x32d120f5, 0x3f92362c, 0x3b533b9b,
	0x031c7626, 0x07dd7b91, 0x0a9e6d48, 0x0e5f60ff,
	0x101840fa, 0x14d94d4d, 0x199a5b94, 0x1d5b5623,
	0xf125760e, 0xf5e47bb9, 0xf8a76d60, 0xfc6660d7,
	0xe22140d2, 0xe6e04d65, 0xeba35bbc, 0xef62560b,
	0xd72d1bb6, 0xd3ec1601, 0xdeaf00d8, 0xda6e0d6f,
	0xc4292d6a, 0xc0e820dd, 0xcdab3604, 0xc96a3bb3,
	0xbd35ad7e, 0xb9f4a0c9, 0xb4b7b610, 0xb076bba7,
	0xae319ba2, 0xaaf09615, 0xa7b380cc, 0xa3728d7b,
	0x9b3dc0c6, 0x9ffccd71, 0x92bfdba8, 0x967ed61f,
	0x8839f61a, 0x8cf8fbad, 0x81bbed74, 0x857ae0c3,
	0x5d86a099, 0x5947ad2e, 0x5404bbf7, 0x50c5b640,
	0x4e829645, 0x4a439bf2, 0x47008d2b, 0x43c1809c,
	0x7b8ecd21, 0x7f4fc096, 0x720cd64f, 0x76cddbf8,
	0x688afbfd, 0x6c4bf64a, 0x6108e093, 0x65c9ed24,
	0x11967be9, 0x1557765e, 0x18146087, 0x1cd56d30,
	0x02924d35, 0x06534082, 0x0b10565b, 0x0fd15bec,
	0x379e1651, 0x335f1be6, 0x3e1c0d3f, 0x3add0088,
	0x249a208d, 0x205b2d3a, 0x2d183be3, 0x29d93654,
	0xc5a71679, 0xc1661bce, 0xcc250d17, 0xc8e400a0,
	0xd6a320a5, 0xd2622d12, 0xdf213bcb, 0xdbe0367c,
	0xe3af7bc1, 0xe76e7676, 0xea2d60af, 0xeeec6d18,
	0xf0ab4d1d, 0xf46a40aa, 0xf9295673, 0xfde85bc4,
	0x89b7cd09, 0x8d76c0be, 0x8035d667, 0x84f4dbd0,
	0x9ab3fbd5, 0x9e72f662, 0x9331e0bb, 0x97f0ed0c,
	0xafbfa0b1, 0xab7ead06, 0xa63dbbdf, 0xa2fcb668,
	0xbcbb966d, 0xb87a9bda, 0xb5398d03, 0xb1f880b4,
};


uint32_t fc_crc32_calc(const uint8_t* buf, uint32_t len)
{
	uint32_t crc = 0;
	
	while(len-- > 0)
	{
		(crc) = ((crc) << 8) ^ fc_crc32_table[((crc) >> 24) ^ *buf++];
	}
	
	return crc;
}

