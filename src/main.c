/*
* @file main.c
* @brief The Shell Entry Point
* The main loop of the shell. It manages the lifecycle of the shell
* Read input -> Parse Input -> Execute
*/

#include "../include/shell.h"
#include "metal.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

void sigchld_handler(int sig)
{ 
    (void)sig;
    while(waitpid(-1,NULL,WNOHANG)>0);
}

int main(void)
{
    char *line; // To store user's typed string here
    char **args; // To store the chopped list of words here
    int status=1; 

    signal(SIGCHLD,sigchld_handler);
    signal(SIGINT,SIG_IGN);

    // The loop that keeps the shell alive
    while(status) 
    {
        printf("hsh> ");
        line=read_line();
        args=parse_input(line);

        if(args[0]!=NULL)
        {

            for(int i=0;args[i]!=NULL;i++)
            {
                if(args[i][0]=='$' && args[i][1]!='\0')
                {
                    char *env_val=getenv(args[i]+1);
                    if(env_val!=NULL)
                    {
                        args[i]=env_val;
                    }
                    else
                    {
                        args[i]="";
                    }
                }
            }

            int is_background=0;
            int last_idx=0;

            while(args[last_idx]!=NULL)
            {
                last_idx++;
            }

            if(last_idx >0 && strcmp(args[last_idx-1],"&")==0)
            {
                is_background=1;
                args[last_idx-1]=NULL;
            }

            int i=0;
            int is_piped=0;

            while(args[i]!=NULL)
            {
                if(strcmp(args[i],"|")==0)
                {
                    args[i]=NULL;
                    char **args2=&args[i+1];

                    execute_piped_commands(args,args2);
                    is_piped=1;
                    break;
                }
                i++;
            }

            if(!is_piped)
            {
                if(execute_builtins(args))
                {
                    if(strcmp(args[0],"exist")==0)
                    {
                        status=0;
                    }
                }
                else
                {
                    char *out_file=NULL;
                    char *in_file=NULL;
                    int cmd_idx=0;

                    int is_time=0;
                    unsigned long long start_ticks=0;
                    struct rusage usage_start;

                    if(args[0]!=NULL && strcmp(args[0],"time")==0)
                    {
                        is_time=1;
                        cmd_idx=1;
                        getrusage(RUSAGE_CHILDREN,&usage_start);
                        start_ticks=metal_get_ticks();
                    }

                    // Output redirecrtion
                    for(int j=cmd_idx;args[j]!=NULL;j++)
                    {
                        if(strcmp(args[j],">")==0)
                        {
                            if(args[j+1]!=NULL)
                            out_file=args[j+1];
                            args[j]=NULL;
                            break;
                        }
                    }

                    // Input redirection
                    for(int j=cmd_idx;args[j]!=NULL;j++)
                    {
                        if(strcmp(args[j],"<")==0)
                        {
                            if(args[j+1]!=NULL)
                            in_file=args[j+1];
                            args[j]=NULL;
                            break;
                        }
                    }
                
                    if(args[cmd_idx]!=NULL)
                    {
                        char *path=find_in_path(args[cmd_idx]);
                        if(path)
                        {
                            execute_command(path,&args[cmd_idx],is_background,out_file,in_file);
                        }
                        else
                        {
                            fprintf(stderr,"hsh: command not found: %s\n",args[cmd_idx]);
                        }
                    }

                    if(is_time)
                    {
                        unsigned long long end_ticks=metal_get_ticks();
                        struct rusage usage_end;
                        getrusage(RUSAGE_CHILDREN,&usage_end);

                        unsigned long long freq=metal_get_freq();
                        if(freq==0)freq=1;
                        double elapsed_sec=(double)(end_ticks-start_ticks)/freq;

                        long long max_rss_kb=(usage_end.ru_maxrss)/1024;
                        long page_faults=usage_end.ru_majflt-usage_start.ru_majflt;
                        long page_reclaims=usage_end.ru_minflt-usage_start.ru_minflt;

                        printf("\n===========[HARDWARE & SYSTEM PROFILER]==============");
                        printf(" CPU Ticks : %llu\n",(end_ticks-start_ticks));
                        printf(" Latency : %.6f seconds\n",elapsed_sec);
                        printf(" Peak Memory RSS : %lld KB\n",max_rss_kb);
                        printf(" Page Faults(I/O) : %ld\n",page_faults);
                        printf(" Page reclaims : %ld\n",page_reclaims);
                        printf("========================================================\n");

                    }
                }
            }
        }
        free(line);
    }
    return 0;
}