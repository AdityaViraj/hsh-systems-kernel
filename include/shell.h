// In C , we need a header file to share functions across files

#ifndef SHELL_H
#define SHELL_H

char *read_line(void);
char **parse_input(char *line);
char execute_builtins(char **args);
char *find_in_path(char *command);
int execute_command(char *path,char **args,int is_background,char *out_filed,char *in_file);
int execute_piped_commands(char **args1,char **args2);

#endif

