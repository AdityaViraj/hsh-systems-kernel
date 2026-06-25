#include "../include/shell.h"
#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE // For getline

char *read_line(void)
{
    char *line=NULL;
    size_t bufsize=0; //getline will allocate memory for us

    if(getline(&line,&bufsize,stdin)==-1)
    {
        if(feof(stdin))
        {
            exit(EXIT_SUCCESS); // Handle EOF
        }
        else
        {
            perror("hsh:getline");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}