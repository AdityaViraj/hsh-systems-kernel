# hsh: A Hardware Aware Unix Shell

`hsh` is a custom, Unix shell written in C. It serves as an exploration of the interface between user space applications and the Linux/Unix kernel.

## 🛠️ Tech Stack
- **Language:** C 
- **Toolchain:** GCC, Git, Make
- **Architecture:** POSIX compliant process control
- **System Calls:** `fork`, `execvp`, `wait`, `strtok`

## How It Works
1. **REPL:** The shell runs an infinite loop, printing `hsh> ` and waiting for input.
2. **Parsing:** User input is tokenised using `strtok`, which overwrites space characters with null terminators (`\0`) in place to save memory.
3. **Execution:** The shell uses `fork()` to clone the current process. The child process uses `execvp()` to replace its own memory map with the requested system binary, while the parent process uses `wait()` to synchronise execution.

## Usage
### Prerequisites
- GCC compiler
- A Unix like environment

![Shell Running ls -l command](assets/Screenshot%202026-06-16%20at%203.04.24%20PM.jpg)\

### Build & Run
```bash
git clone [https://github.com/adityaviraj/hsh-systems-kernel.git](https://github.com/adityaviraj/hsh-systems-kernel.git)
cd hsh-systems-kernel
gcc main.c -o hsh
./hsh