/**
 * @file path_utils.c
 * @brief Search for binaries in system PATH.
 */
#include "../include/shell.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char *find_in_path(char *command) 
{
    char *path = getenv("PATH");
    char *path_copy = strdup(path); // Duplicate so we can slice it up
    char *dir = strtok(path_copy, ":");
    static char full_path[1024];

    while (dir != NULL) 
    {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
        // access(path, X_OK) checks if the file exists and is executable
        if (access(full_path, X_OK) == 0) 
        {
            free(path_copy);
            return full_path;
        }

        dir = strtok(NULL, ":");
    }
    free(path_copy);
    return NULL;
}