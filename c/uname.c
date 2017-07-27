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

#include <sys/utsname.h>

#include <netdb.h>


int gethostname (char *name, size_t len)
{
    struct utsname uts;

    if (uname(&uts) == -1)
        return -1;
    if (len > sizeof(uts.nodename))
    {
        name[sizeof(uts.nodename)] = '\0';
        len = sizeof(uts.nodename);
    }
    strncpy(name, uts.nodename, len);

    return 0;
}    

int getaddrinfo()
{
    struct addrinfo hints, *info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_CANONNAME|AI_CANONIDN;
    hints.ai_family = AF_UNSPEC;

    getaddrinfo(hostname, NULL, &hints, &info);
}    

int main(void)
{
    char hostname[255];

    gethostname(hostname, sizeof(hostname));

    printf("hostname %s\n", hostname);
}

