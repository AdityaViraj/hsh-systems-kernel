// This hadndles cd and exit without needing to fork.

/*
* @file builtins.c
* @brief Handles shell built in commands
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

int execute_builtins(char **args)
{
    if(strcmp(args[0],"exit")==0) return 1;  // Handled by main
    
    if(strcmp(args[0],"cd")==0)
    {
        if(args[1]==NULL) 
        // To print stderr we need fprintf as printf alwaya prints the standard outputs.
        fprintf(stderr,"hsh:cd missing argument\n");
        else if (chdir (args[1])!=0) 
        perror("hsh");
        return 1; //Bulit in handled
    }
    return 0; //Not built in 
}


    
    
    
    
