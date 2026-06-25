// In C , we need a header file to share functions across files

#ifndef SHELL_H
#define SHELL_H

char *read_line(void);
char **parse_input(char *line);
char execute_builtins(char **args);
char *find_in_path(char *command);
int execute_command(char *path,char **args);

#endif
