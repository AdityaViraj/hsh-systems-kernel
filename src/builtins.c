// This hadndles cd and exit without needing to fork.

/*
* @file builtins.c
* @brief Handles shell built in commands
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "metal.h"

int execute_builtins(char **args)
{
    if(args[0]==NULL)
    {
        return 0;
    }

    if(strcmp(args[0],"exit")==0)
    {
        metal_exit(0); // hardware level exit
    }
    
    if(strcmp(args[0],"cd")==0)
    {
        if(args[1]==NULL)
        {
            fprintf(stderr,"hsh: cd missing argument\n");
        }
        else if (chdir(args[1])!=0)
        {
            perror("hsh");
        }
        return 1; // Built in handled
    }
    return 0; // not a built in
}
    

    
    
    
