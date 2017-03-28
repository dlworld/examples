/**
 * @par Copyright 2017 Wuhan OS-Easy Cloud Computing Co. Ltd.
 * @file fcadm.h
 * @brief fcadm header.
 * @author LastRitter
 * @version 1.0.0
 * @date 2017-01-19
 */
#ifndef __FILE_CACHE_ADMIN_MHHGGG_H__
#define __FILE_CACHE_ADMIN_MHHGGG_H__



//--------------------------------------------------------------------------------------------------------------------
#define FC_LOCAL				static

#ifdef _MSC_VER_							// for MSVC
#define FC_FORCE_INLINE			__forceinline
#elif defined __GNUC__						// for gcc on Linux/Apple OS X
#define FC_FORCE_INLINE			__inline__ __attribute__((always_inline))
#else
#define FC_FORCE_INLINE			inline
#endif

#define FC_LOCAL_INLINE			FC_LOCAL FC_FORCE_INLINE

#if defined(__KERNEL__) && defined(__linux__)
#define FC_LIKELY(expr)			likely((expr))
#define FC_UNLIKELY(expr)		unlikely((expr))
#else
#define FC_LIKELY(expr)			(expr)
#define FC_UNLIKELY(expr)		(expr)
#endif

#define FC_PACKED				__attribute__((__packed__))

#define FC_NUM_OF(a)			((int)(sizeof(a)/sizeof((a)[0])))


/**
 * @brief FC_OFFSET_OF, 计算MEMBER成员在TYPE结构体中的偏移量
 *
 * @param type				(input)
 *
 * @param member			(input)
 *
 * @return 偏移量
 */
#define FC_OFFSET_OF(type, member)	((unsigned long)(&(((type*)0)->member)))
  
  
/**
 * @brief FC_CONTAINER_OF, 计算链表元素的起始地址
 *
 * @param ptr				(input)type结构体中的链表指针
 *
 * @param type				(input)结构体类型
 *
 * @param member			(input)链表成员名称
 *
 * @return 起始地址
 */
#define FC_CONTAINER_OF(ptr, type, member)	((type*)((char*)(ptr) - FC_OFFSET_OF(type, member)))
  

#define FC_ALIGN_DOWN(a, size)	((a) & (~((size) - 1)) )
#define FC_ALIGN_UP(a, size)	(((a) + (size) - 1) & (~((size) - 1)))

#define FC_TRUE					(0 == 0)
#define FC_FALSE				(0 == 1)


//--------------------------------------------------------------------------------------------------------------------

#define FC_UINT64_MAX			(~((uint64_t)0))

#define FC_SIZE_1K				1024UL
#define FC_SIZE_1M				(FC_SIZE_1K * FC_SIZE_1K)
#define FC_SIZE_1G				(FC_SIZE_1M * FC_SIZE_1K)
#define FC_SIZE_1T				(FC_SIZE_1G * FC_SIZE_1K)


#define FC_SECTOR_SHIFT			9UL
#define FC_SECTOR_SIZE			(1 << (FC_SECTOR_SHIFT))



//--------------------------------------------------------------------------------------------------------------------

#define FC_SECT_TO_SIZE(sec)	((sec) << (FC_SECTOR_SHIFT))
#define FC_SECT_TO_MEGA(sec)	((sec) >> (20 - FC_SECTOR_SHIFT))

#define FC_NET_IP_QUAD(addr)	((unsigned char *)&addr)[0], ((unsigned char *)&addr)[1], \
  	((unsigned char *)&addr)[2], ((unsigned char *)&addr)[3]


#define FC_STR_TO_UINT64(a, b, c, d, e, f, g, h)	((((uint64_t)(a)) << 0) | (((uint64_t)(b)) << 8) | (((uint64_t)(c)) << 16) \
		| (((uint64_t)(d)) << 24) | (((uint64_t)(e)) << 32) | (((uint64_t)(f)) << 40) | (((uint64_t)(g)) << 48) | (((uint64_t)(h)) << 56))



//--------------------------------------------------------------------------------------------------------------------

#if defined(__KERNEL__) && defined(__linux__)
#define FC_UINT64_STR_FORMAT	"%llu"
#else
#define FC_UINT64_STR_FORMAT	"%lu"
#endif



//--------------------------------------------------------------------------------------------------------------------
#define FC_SNPRINT_RET(buf, len, offset, fmt, ...) \
do { \
	(offset) += snprintf(&(buf)[(offset)], (len) - (offset), fmt, ##__VA_ARGS__); \
	if((offset) >= ((len) - 1)) \
		return ((len) - 1); \
} while(0)

#define FC_SNPRINT_BREAK(buf, len, offset, fmt, ...) \
do { \
	(offset) += snprintf(&(buf)[(offset)], (len) - (offset), fmt, ##__VA_ARGS__); \
	if((offset) >= ((len) - 1)) \
		break; \
} while(0)


#define FC_SNPRINT_INT_RET(buf, len, offset, str, val) \
	FC_SNPRINT_RET((buf), (len), (offset), "%-25s %d\n", (str), (val))

#if defined(__KERNEL__) && defined(__linux__)

#define FC_SNPRINT_UINT64_RET(buf, len, offset, str, val) \
	FC_SNPRINT_RET((buf), (len), (offset), "%-25s %llu\n", (str), (val))

#else

#define FC_SNPRINT_UINT64_RET(buf, len, offset, str, val) \
	FC_SNPRINT_RET((buf), (len), (offset), "%-25s %lu\n", (str), (val))

#endif


typedef int (*fc_print_t)(const char* format, ...);

typedef int (*fc_sprint_t)(void* data, const char* format, ...);


#ifndef _GNU_SOURCE
#define _GNU_SOURCE /* for O_DIRECT */
#endif

#ifndef __USE_FILE_OFFSET64
#define __USE_FILE_OFFSET64
#endif

#ifndef __USE_LARGEFILE64
#define __USE_LARGEFILE64
#endif

#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif


//********************************************************************************************************************

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <malloc.h>
#include <getopt.h>
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>  
#include <dirent.h>
#include <syslog.h>
#include <time.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/signalfd.h>  
#include <sys/stat.h>
#include <sys/vfs.h>

#include <netinet/in.h>
#include <arpa/inet.h>



//********************************************************************************************************************

/**
 * @brief fc错误码
 */
typedef enum
{
	FC_INVALID_PARAM 	= EINVAL, 
	FC_NO_MEMORY		= ENOMEM, 
	FC_NO_DEVICE		= ENODEV, 
	FC_DEVICE_BUSY		= EBUSY, 
	FC_TRY_AGAIN		= EAGAIN, 
	FC_INTERRUPTED		= EINTR, 
	FC_NOT_TYPEWRITER	= ENOTTY, 
	FC_BAD_ADDRESS		= EFAULT, 
	FC_NOT_PERMITTED	= EPERM, 
	FC_NO_SUCH_FILE		= ENOENT, 
	FC_IO_ERROR			= EIO, 
	
	FC_OK				= 0, 
	FC_FAILED			= ~((int)0), 
	
} fc_errno_t;


//--------------------------------------------------------------------------------------------------------------------

#define FC_FUNC_STR		"%-15s"

#if defined(__KERNEL__) && defined(__linux__)
#define FC_LOG_ERR		KERN_ERR
#define FC_LOG_WAR		KERN_WARNING
#define FC_LOG_INF		KERN_INFO
#define FC_LOG_DBG		KERN_ERR

#define FC_LOG(level, format, ...)		printk(level format, ##__VA_ARGS__)



#else /* defined(__KERNEL__) && defined(__linux__) */

#ifdef FC_USE_SYSLOG
#define FC_LOG_ERR		LOG_ERR
#define FC_LOG_WAR		LOG_WARNING
#define FC_LOG_INF		LOG_INFO
#define FC_LOG_DBG		LOG_DEBUG

#define FC_LOG(level, format, ...)		\
	do \
	{ \
		printf(format, ##__VA_ARGS__); \
		syslog(level, format, ##__VA_ARGS__); \
	} while(0)


FC_LOCAL_INLINE int fc_log_init(const char* name, int facility)
{
	openlog(name, LOG_CONS | LOG_PID, facility);
	return FC_OK;
}


FC_LOCAL_INLINE int fc_log_exit(void)
{
	closelog();
	return FC_OK;
}


#else /* FC_USE_SYSLOG */

#define FC_LOG_ERR		0
#define FC_LOG_WAR		1
#define FC_LOG_INF		2
#define FC_LOG_DBG		3

#define FC_LOG(level, format, ...)		printf(format, ##__VA_ARGS__)


FC_LOCAL_INLINE int fc_log_init(const char* name, int facility)
{
	return FC_OK;
}


FC_LOCAL_INLINE int fc_log_exit(void)
{
	return FC_OK;
}


#endif /* FC_USE_SYSLOG */
#endif /* defined(__KERNEL__) && defined(__linux__) */


//--------------------------------------------------------------------------------------------------------------------

#define FC_ERR(format, ...) \
	do \
	{ \
		FC_LOG(FC_LOG_ERR, FC_FUNC_STR " : " format, __func__, ##__VA_ARGS__); \
		FC_LOG(FC_LOG_ERR, "(%s:%d %s)\n\n", __FILE__, __LINE__, __DATE__); \
	} while(0)

#define FC_WAR(format, ...) \
	do \
	{ \
		FC_LOG(FC_LOG_WAR, FC_FUNC_STR " : " format, __func__, ##__VA_ARGS__); \
	} while(0)


#define FC_INF(format, ...) \
	do \
	{ \
		FC_LOG(FC_LOG_INF, FC_FUNC_STR " : " format, __func__, ##__VA_ARGS__); \
	} while(0)


#ifdef FC_DEBUG
#define FC_BUG(format, ...) \
	do \
	{ \
		FC_LOG(FC_LOG_ERR, "%-20s : BUG ! ! ! %s line %d, %s.\n", __func__, __FILE__, __LINE__, __DATE__); \
		FC_LOG(FC_LOG_ERR, "%-20s : " format "\n", __func__, ##__VA_ARGS__); \
	} while(0)
#else /* FC_DEBUG */
#define FC_DBG(format, ...)	do{} while(0)
#endif /* FC_DEBUG */

#define FC_OUT(format, ...)		FC_LOG(FC_LOG_INF, format, ##__VA_ARGS__)




//********************************************************************************************************************

/**
 * @brief FC_IS_POWER2, 判断数字num是否是2的N次方
 *
 * @param num				(input)被判断的非负整数
 *
 * @return 是则返回FC_TRUE，否则返回FC_FALSE
 */
FC_LOCAL_INLINE uint64_t FC_IS_POWER2(uint64_t num)
{
	if(num < 1)
		return FC_FALSE;
	
	return (num & (num -1)) == 0 ? FC_TRUE : FC_FALSE;
}


/**
 * @brief FC_FAST_LOG2, 快速计算一个值为2^N的正整数以2为底的对数
 *
 * @param x					(input)
 *
 * @return 对数值
 */
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
FC_LOCAL_INLINE int FC_FAST_LOG2(int x)
{
    float fx;
    unsigned long ix;

    fx = (float)x;
    ix = *(unsigned long*)&fx;

    return ((ix >> 23) & 0xFF) - 127;
}
#pragma GCC diagnostic warning "-Wstrict-aliasing"


/**
 * @brief FC_ENDIAN_SWAP64, swap 64 bit.
 */
FC_LOCAL_INLINE uint64_t FC_ENDIAN_SWAP64(uint64_t value)
{
	return (((value >>  0) & 0xFF) << 56) | (((value >>  8) & 0xFF) << 48) | (((value >> 16) & 0xFF) << 40) 
		| (((value >> 24) & 0xFF) << 32) | (((value >> 32) & 0xFF) << 24) | (((value >> 40) & 0xFF) << 16) 
		| (((value >> 48) & 0xFF) <<  8) | (((value >> 56) & 0xFF) <<  0);
}


/**
 * @brief FC_ENDIAN_SWAP32, swap 32 bit.
 */
FC_LOCAL_INLINE uint32_t FC_ENDIAN_SWAP32(uint32_t value)
{
	return ((value & 0xFF000000) >> 24) | ((value & 0x00FF0000) >> 8) | 
		((value & 0x0000FF00) << 8) | ((value & 0x000000FF) << 24);
}


/**
 * @brief FC_ENDIAN_SWAP16, swap 16 bit.
 */
FC_LOCAL_INLINE uint16_t FC_ENDIAN_SWAP16(uint16_t value)
{
	return ((value & 0xFF00) >> 8) | ((value & 0x00FF) << 8);
}


/**
 * @brief fc_str_to_size, parse size value from string.
 */
int fc_str_to_size(const char* str, uint64_t* val);


/**
 * @brief fc_str_to_size, parse size value frome string.
 */
const char* fc_size_to_str(uint64_t val, char* str, int len);

const char* fc_char_to_str(char* ch, int num, char* str, int len);


#if defined(__KERNEL__) && defined(__linux__)
extern void fc_malloc_times_add(void);
#endif /* defined(__KERNEL__) && defined(__linux__) */

FC_LOCAL_INLINE void* fc_malloc(int len)
{
#if defined(__KERNEL__) && defined(__linux__)
	fc_malloc_times_add();
	return vmalloc(len);
#else /* defined(__KERNEL__) && defined(__linux__) */
	return malloc(len);
#endif /* defined(__KERNEL__) && defined(__linux__) */
}


#if defined(__KERNEL__) && defined(__linux__)
extern void fc_mfree_times_add(void);
#endif /* defined(__KERNEL__) && defined(__linux__) */

FC_LOCAL_INLINE void fc_free(void* ptr)
{
#if defined(__KERNEL__) && defined(__linux__)
	fc_mfree_times_add();
	vfree(ptr);
#else /* defined(__KERNEL__) && defined(__linux__) */
	free(ptr);
#endif /* defined(__KERNEL__) && defined(__linux__) */
}


#if defined(__KERNEL__) && defined(__linux__)
/**
 * @brief fc_signal_check, 检查当前是否由不能被忽略的信号
 *
 * @return 如果有不能忽略的信号则返回FC_TRUE，否则返回FC_FALSE
 */
FC_LOCAL_INLINE int fc_signal_check(void)
{
	if(signal_pending(current))
	{
		const uint32_t task_flags = task_thread_info(current)->flags;
		const uint64_t signal_set = current->pending.signal.sig[0];

		if((signal_set & ((1 << SIGTRAP) | (1 << SIGCONT))) != 0)
		{
//			FC_INF("debug trap or continue signal, continue, task_flags 0x%08X, signal_set 0x%016llX.\n", 
//				task_flags, signal_set);
			return FC_FALSE;
		}
		else
		{
			FC_INF("not debug trap or continue signal, exit, task_flags 0x%08X, signal_set 0x%016llX.\n", 
				task_flags, signal_set);
			return FC_TRUE;
		}
	}
	
	return FC_FALSE;
}

#else /* defined(__KERNEL__) && defined(__linux__) */

int fc_epoll_noblock_set(int fd);
int fc_epoll_fd_add(int epollfd, int fd, int oneshot);
int fc_epoll_oneshot_reset(int epollfd, int fd);

int fc_hostname_get(char* hostname, int len);

#endif /* defined(__KERNEL__) && defined(__linux__) */



uint32_t fc_crc32_calc(const uint8_t* buf, uint32_t len);



//********************************************************************************************************************

#define DMD_CMD_CHAR_BASE	'A'

#define DMD_CMD_CHAR(code)	(DMD_CMD_CHAR_BASE + (code))
#define DMD_CMD_CODE(char)	((char) - DMD_CMD_CHAR_BASE)

#define DMD_CMD_CHAR_MAX	DMD_CMD_CHAR(FC_CMD_MAX)


typedef enum fc_cmd_code
{
	FC_CMD_SHOW		= 0, 
	FC_CMD_LOAD, 
	FC_CMD_DROP, 
	FC_CMD_LOCK, 
	FC_CMD_UNLOCK, 
	
	FC_CMD_TEST, 
	FC_CMD_HELP, 
	
	FC_CMD_MAX, 
} fc_cmd_code_t;



//--------------------------------------------------------------------------------------------------------------------

typedef struct fc_addr_area
{
	size_t off;
	size_t len;
	
} fc_addr_area_t;


typedef struct fc_params
{
	int is_break;
	
	int cmd;
	
	char path[2048];
	int fd;
	
	uint8_t* file_mem;
	
	size_t file_size;
	size_t page_size;
	size_t page_num;
	
	int aa_num;
	fc_addr_area_t aa_map[1024];
	
} fc_params_t;


int fc_params_parse(fc_params_t* const params, int argc, char* argv[]);


//--------------------------------------------------------------------------------------------------------------------

typedef int (*fc_cmd_cb_t)(fc_params_t* const params);

typedef struct dmd_cmd
{
	char name [256];
	int cmd;
	
	fc_cmd_cb_t func;
	
} dmd_cmd_t;


int dmd_cmd_run(fc_params_t* const params);





#endif /* __FILE_CACHE_ADMIN_MHHGGG_H__ */

