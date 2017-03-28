/*
 * =====================================================================================
 *
 *       Filename:  hugepage.c
 *
 *    Description: 
 *
 *        Version:  1.0
 *        Created:  2016年12月23日 17时21分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define HUGETLBFS_MAGIC       0x958458f6

int main(int argc, char *argv[])
{
    int ret, fd;
    struct statfs fs;
    char *path;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <num>...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    path = argv[1];
    fd = open(path, O_RDWR);
    if (fd == -1)
    {        
        fprintf(stderr, "open %s failed\n", argv[1]);
    	do {
           ret = statfs(path, &fs);
	} while (ret != 0 && errno == EINTR);
    } else {       
    	do {
           ret = fstatfs(fd, &fs);
	} while (ret != 0 && errno == EINTR);
    }

    if (ret == 0) {
        fprintf(stderr, "file_type 0x%lx, size 0x%lx\n", fs.f_type, fs.f_bsize);
    } else {
   	handle_error("statfs");
        exit(EXIT_FAILURE);
    }

    return ret;
}    
