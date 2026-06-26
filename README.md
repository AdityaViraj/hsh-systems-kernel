# '**hsh**': A Hardware Aware Shell

A personal project to build a command line interpreter from scratch in C. This serves as an interface between the User and the Operating System.

## How it is built ?

This shell runs on the concept of REPL (Read Evaluate Print Loop). This continously reads the input then tokenises and manages process lifecycles.
- Process Management : Used fork() to create the child process and execvp() to replace the child's memory with the folloeing binaries. Also used waitpid() to synchronise execution.
- Path resolution : The shell retirves the PATH of the system using the getenv() and uses access() to locate the binary files.
- Memory Management : Handled dynamic allocation for all the tokens and ensuring every command cycle cleans up to prevent memory leaks.

## Structure

| File Name | Its Role |
| --- | --- |
| `main.c` | The main entry point of the shell that manages the REPL loop. |
| `shell.h` | It defines the global variables and function prototypes. |
| `parser.c` | It handles the splitting of the user given input into individual commands and arguments. It uses strtok() to tokenise the input string based on whitespace and some specific special characters. |
| `executor.c` | It handles the execution of commands both built in and external. Uses fork() to create a new process for each command and execvp() to execute that command in the child process. |
| `path_utils.c` | It searches for the given command in the system's PATH environment variable. Uses getenv() to retrieve the PATH variable and access() to check if that particular command exists in the directories. |
| `builtins.c` | It implements the built in commands like cd , help and exit.|
| `get_line.c` | It buffers raw input from the terminal for processing. |
| `Makefile` | It compiles the source code files into direct executable binary by defining the rules and dependencies for all the source code files.|

## Build & Run

```bash
git clone https://github.com/AdityaViraj/hsh-systems-kernel
cd hsh-systems-kernel
make clean && make
./hsh

```

**NOTE:** You might see a security warning because this is a custom compiled binary. To bypass this issue right click the `hsh` file , select Open  and click  Open again in the confirmation window.
