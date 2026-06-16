#include <stdio.h>
#include <string.h>

int main(void)
{
    // 1. A continous array of bytes in memory
    char command[]="ls -l /";

    printf("Original memory block: %s\n\n",command);

    // 2. strtok findds the first space , overwrites it with '\0' and returns a pointer
    char *piece=strtok(command," ");
    printf(piece);

    // 3. Loop until we run out of space
    while(piece!=NULL)
    {
        printf("Sliced piece found at memeory address %p:%s\n",(void*)piece,piece);
        
        piece = strtok(NULL," ");

    }

    return 0;
}

// strtok does not copy data or create new memory. It physically walks down your existing array , finds the space charecter (' ') and overwrites it with a \0 (the invisible C symbol for "stop reading here").
