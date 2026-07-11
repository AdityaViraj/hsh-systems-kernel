#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
#define ARENA_SIZE 4096

static char arena[ARENA_SIZE];
static size_t arena_offset=0;

void arena_reset(void)
{
    arena_offset=0;
}

void *arena_alloc(size_t size)
{
    if(arena_offset+size>ARENA_SIZE)
    {
       fprintf(stderr,"hsh: arena out of memory\n");
       exit(EXIT_FAILURE); 
    }
    void *ptr=&arena[arena_offset];
    arena_offset+=size;
    return ptr;
}

char **parse_input(char *line)
{
    arena_reset();

    size_t bufsize=TOK_BUFSIZE;
    size_t position=0;

    char **tokens=arena_alloc(bufsize*sizeof(char *));
    char *token;

    token=strtok(line,TOK_DELIM);
    while(token!=NULL)
    {
        tokens[position]=token;
        position++;

        if(position>=bufsize)
        {
            bufsize+=TOK_BUFSIZE;

            char **new_tokens=arena_alloc(bufsize*sizeof(char *));
            memcpy(new_tokens,tokens,position*sizeof(char *));
            tokens=new_tokens;
        }
        token=strtok(NULL,TOK_DELIM);
    }
    tokens[position]=NULL;
    return tokens;
}