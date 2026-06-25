/*
* @file main.c
* @brief The Shell Entry Point
* The main loop of the shell. It manages the lifecycle of the shell
* Read input -> Parse Input -> Execute
*/

#include "../include/shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *line; // To store user's typed string here
    char **args; // To store the chopped list of words here
    int status=1; 

    // The loop that keeps the shell alive
    while(status) 
    {
        printf("hsh> "); // The prompt

        // Custom function : We wait for keyboard input
        line=read_line();

        // Custom function : Split "ls -l" into ["ls","-l"]
        args = parse_input(line);
        printf("Debug: Program is '%s', First Arg is '%s'\n", args[0], args[1]);

        // Did user enter anything ?
        if(args[0]!=NULL)
        {
            if(execute_builtins(args))
            { 
                // Check if its a built in command (cd,exit)
                if(strcmp(args[0],"exit")==0) 
                status=0; // If exit , kill the loop
            }
                else
                {
                    // If not built in , look for it in the system (PATH)
                    char *path=find_in_path(args[0]);
                    if(path)
                    {
                        execute_command(path,args); // Run the program
                    }
                    else
                    {
                        fprintf(stderr,"hsh: command not found:%s\n",args[0]); // Print to error stream
                    }
                }
            }
            free(line); // Clean up memory
            free(args); // Clean up memory
        }
        return 0; 
    }

    
