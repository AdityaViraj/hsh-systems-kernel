/*
🧬 Core Concept: What is fork()?
In standard programming, code runs sequentially line-by-line. But fork() is a system call that tells the operating system kernel: "Clone this running program right now." The moment fork() executes, your single program splits into two identical programs running at the exact same time:
The Parent Process: The original program that called fork().
The Child Process: The brand-new clone created by the kernel.
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    pid_t pid; // Variable to store the process ID

    printf("--- BEFORE FORK: There is only ONE process ---\n");
    printf("My process ID(PID) is: %d\n", getpid());

    // The Magic Line: The program clones itself here!
    pid = fork();

    // -- AFTER FORK: Both parent and child execute the code below simultaneously ---

    if (pid < 0)
    {
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if (pid == 0)
    {
        // Inside the CHILD process
        printf("\n[CHILD] Hello! I am the cloned process.\n");
        printf("[CHILD] My unique PID is: %d\n", getpid());
        printf("[CHILD] My parent PID is: %d\n", getppid()); // 
    }
    else
    {
        // Inside the PARENT process
        printf("\n[PARENT] Hello! I am the original process.\n");
        printf("[PARENT] My PID is: %d\n", getpid()); 
    }

    printf("--- END: Process %d is exiting ---\n", getpid());
    return 0;
}