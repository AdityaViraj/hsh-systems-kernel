// Executor functions for handling command execution

/* This code is the bridge between our C program and the hardware.
   It uses fork() to create a clone and execvp() to tranform that clone into the actual command (like ls or gcc)
*/

/*
* @file executor.c
* @brief Handles process creation and execution
*/

#include "../include/shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
* @brief Executes a command by forking and using execvp
* @param path The full path to the binary
* @param args the NULL terminated array of arguments
* @return 0 on success , -1 on failure
*/

int execute_command(char *path,char **args)
{
   pid_t pid=fork();

   if(pid==0)
   {
      // -- Child Process
      // execvp replaces the current process with the new program
      if(execvp(path,args)==-1)
      {
         perror("hsh:execvp failed");
         exit(EXIT_FAILURE);
      }
   }
   else if(pid<0)
   {
      perror("hsh: fork failed");
      return -1;
   }
   else
   {
      // Parent Process
      //Wait for the child to finish so we don't leave "zombie processes".
      int status;
      waitpid(pid,&status,0);
   }
   return 0;
}





