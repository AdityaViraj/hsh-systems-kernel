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
#include <fcntl.h>

/*
* @brief Executes a command by forking and using execvp
* @param path The full path to the binary
* @param args the NULL terminated array of arguments
* @return 0 on success , -1 on failure
*/

int execute_command(char *path,char **args, int is_background,char *out_file,char *in_file)
{
   pid_t pid=fork();

   if(pid==0)
   {
      // -- Child Process
      if(out_file)
      {
         signal(SIGINT,SIG_DFL);

         int fd=open(out_file,O_WRONLY|O_CREAT|O_TRUNC,0644);
         if(fd<0)
         {
            perror("hsh: redirection failed");
            exit(EXIT_FAILURE);
         }
         dup2(fd,STDOUT_FILENO);
         close(fd);
      }

      if(in_file)
      {
         int fd_in=open(in_file,O_RDONLY);
         if(fd_in<0)
         {
            perror("hsh: input redirection failed");
            exit(EXIT_FAILURE);
         }
         dup2(fd_in,STDIN_FILENO);
         close(fd_in);
      }

      // execvp replaces the current process with the new program
      if(execvp(path,args)==-1)
      {
         perror("hsh: execvp failed");
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
      if(is_background)
      {
         printf("[+] Started background job with PID: %d\n",pid);
         return 0;
      }
      else
      {
         int status;
         waitpid(pid,&status,0);

         if(WIFEXITED(status))
         {
            return WEXITSTATUS(status);
         }
         return 1;
      }
   }
   return 0;
}

int execute_piped_commands(char **args1 , char **args2)
{
   int pipefd[2];
   pid_t p1,p2;

   if(pipe(pipefd)<0)
   {
      perror("hsh: pipe failed!");
      return -1;
   }

   p1=fork();
   if(p1==0)
   {
      close(pipefd[0]);
      dup2(pipefd[1],STDOUT_FILENO);
      close(pipefd[1]);

      if(execvp(args1[0],args1)==-1)
      {
         perror("hsh: execvp failed for cmd1");
         exit(EXIT_FAILURE);
      }
   }

   p2=fork();
   if(p2==0)
   {
      close(pipefd[1]);
      dup2(pipefd[0],STDIN_FILENO);
      close(pipefd[0]);

      if(execvp(args2[0],args2)==-1)
      {
         perror("hsh: execvp failed for cmd2");
         exit(EXIT_FAILURE);
      }
   }

   close(pipefd[0]);
   close(pipefd[1]);

   waitpid(p1,NULL,0);
   waitpid(p2,NULL,0);
   
   return 0;
}
