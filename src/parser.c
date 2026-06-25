#include "../include/shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a" 
// strtok works by checking every char of our string the charecters in TOK_DELIM 
// The space in the starting is very important 


/*
 parse_input - Splits string into tokens based on the whitespace
 @line: The raw input string from the user
 Return: A null terminated array of strings 
*/

char **parse_input(char *line)
{
    size_t bufsize=TOK_BUFSIZE;
    size_t position=0;
    char **tokens=malloc(bufsize * sizeof(char *));
    char *token;


    if(!tokens)
    {
        fprintf(stderr,"hsh:allocation error\n");
        exit(EXIT_FAILURE);
    }

    token=strtok(line,TOK_DELIM);
    printf("Debug: The first token is '%s'\n",token);
    while(token!=NULL)
    {
        tokens[position]=token;
        position++;
        
        // If we exceed , we reallocate
        if(position>=bufsize)
        {
            bufsize+=TOK_BUFSIZE;
            tokens=realloc(tokens,bufsize * sizeof(char *));
            if(!tokens)
            {
                fprintf(stderr,"hsh:allocation error");
                exit(EXIT_FAILURE);
            }
        }
        token=strtok(NULL,TOK_DELIM);
    }
    // NULL pointer to know where the array ends
    tokens[position]=NULL;
    return tokens;
}