#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void main()
{
    char *ptr = malloc(sizeof(char));

    *ptr = 'a';
    free(ptr);
    //ptr = NULL;
    free(ptr);
}
