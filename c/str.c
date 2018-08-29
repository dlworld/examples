/*
 * =====================================================================================
 *
 *       Filename:  uname.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017年06月30日 11时52分36秒
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
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(void)
{
    unsigned long l = 0;
    char c;

    while ((c = getchar()) != EOF) {
        printf("%c ", c);
    }

    printf("hostname %s-%ld\n", "test", l);
}

