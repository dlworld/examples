/*
 * =====================================================================================
 *
 *       Filename:  open.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017年01月12日 14时02分25秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#define _GNU_SOURCE

#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)
    

int main(int argc, char *argv[])
{
    int ret;
    char *name;

    name = argv[1];

    ret = open(name, O_RDWR | O_DIRECT );

    if (ret == -1) {
        printf("open %s failed. error number: %d.\n", name, errno);
        handle_error("open error");
    }

}    
