// REPL (Read-Evaluate-Print loop) which is the foundational heartbeat of every command-line interface on earth

// Building a custom Linux shell means writing your owm command line interpreter application from scratch a programming langauge like C , C++ and Go. Instead of releying on exisiting shells like Bash and Zsh , we build a program that acts as the direct interface between a user and the Linux shell

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define PROMPT "hsh> "
#define BUFFER_SIZE 1024

int main(void)
{
    char input[BUFFER_SIZE];

    while(1)
    {
        printf("%s", PROMPT);
        fflush(stdout);

        // 1. Read input from the keyboard (stdin)
        if(fgets(input,sizeof(input),stdin)==NULL)
        {
            printf("Existing hsh\n");
            break;

        }

        // 2. Strip the hidden newline charecter
        input[strcspn(input,"\n")]=0;

        // 3. Built-in exit command
        if(strcmp(input,"exit")==0) 
        {
            printf("Goodbye!\n");
            break;
        }

        // --- THE PARSING ENGINE ---
        char *args[64]; // To hold upto 64 pointers
        char *token =strtok(input," ");
        int i=0;

        while(token!=NULL)
        {
            args[i]=token; // Store the memory address of the sliced word
            i++;
            token=strtok(NULL," ");
        }

        args[i]=NULL; // We need NULL at the end to to know the array had ended
        

        // If the user just pressed Enter withour typing anything just ask again by looping
        if(args[0]==NULL)
        {
            continue;
        }
        
        // --- THE EXECUTION ENGINE ---
        pid_t pid=fork();

        if(pid<0)
        {
            fprintf(stderr,"Fork failed\n");
        }

        else if(pid==0)
        {
            // CHILD PROCESS
            // We hand the array of pointers to the OS
            if(execvp(args[0],args)==-1)
            {
                perror("hsh error");
            }
            
            // If execvp fails , the child must die here
            exit(EXIT_FAILURE);
        }

        else
        {
            // PARENT PROCESS
            // Sleep until the child finishes executing the command
            wait(NULL);
        }
    }
    return 0;
}
// In modern C  standards , leaving the parenthesis completely empty in int main() tells  the compiler : " This function could take any number of random aruguments." The strict compiler flags running in your VS Code setup prefer a strict defination
//To make the compiler completely happy , we just need to explictly state that main takes exactly zero aruguments by putting insdie the parenthesis