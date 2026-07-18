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

## Requirements

`hsh` is a POSIX shell — it relies on `fork()`, `execvp()`, and raw Linux/Darwin syscalls, so it needs a real POSIX kernel underneath it.

| Platform | Supported? | What you need |
| --- | --- | --- |
| macOS (Intel or Apple Silicon) | ✅ Native | Xcode Command Line Tools (`xcode-select --install`) — provides `clang`, `make` |
| Linux (x86_64 or arm64) | ✅ Native | `build-essential` (Debian/Ubuntu) or equivalent — provides `gcc`/`cc`, `make` |
| Windows | ✅ Via WSL2 only | [WSL2](https://learn.microsoft.com/windows/wsl/install) running Ubuntu (or any Linux distro), then follow the Linux steps inside it |

**Windows note:** `hsh` cannot be built with MSVC or run as a native `.exe` — Windows doesn't have `fork()`, and its syscall ABI is completely different from the Linux/Darwin one this shell talks to directly in `metal.h`. WSL2 gives you a real Linux kernel, so the Linux build instructions below work unchanged inside it.

## Build & Run

> ⚠️ **Always build from source — never run a checked-in `hsh` binary.**
> A compiled binary is tied to the exact OS and CPU architecture it was built on (e.g. a binary built on Apple Silicon is macOS-arm64-only and simply won't execute on Linux, Windows/WSL, or even an Intel Mac). There is no binary that runs on "any computer" — `make` is what makes this project portable, not the executable it produces. If you see an `hsh` file already sitting in the repo, delete it and rebuild.

```bash
git clone https://github.com/AdityaViraj/hsh-systems-kernel
cd hsh-systems-kernel
make
./hsh
```

That's it on macOS, Linux, and inside WSL2 — the `Makefile` picks a working compiler (`clang` on macOS, `gcc` on Linux) automatically via the `cc` alias, so there's no platform-specific flag to remember.

If you ever need to force a specific compiler:

```bash
make CC=gcc      # or CC=clang
```

To rebuild from scratch:

```bash
make clean && make
```

To build and immediately drop into the shell in one step:

```bash
make run
```

### macOS Gatekeeper note

Since `hsh` is a locally compiled, unsigned binary, macOS may show a security warning the first time you run it. If that happens: right-click `hsh` in Finder → **Open** → **Open** again in the confirmation dialog. This only needs to be done once.
