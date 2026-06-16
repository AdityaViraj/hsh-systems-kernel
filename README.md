# hsh: A Hardware Aware Unix Shell

`hsh` is a custom, bare-metal Unix shell written entirely in C from scratch. It is designed to bridge the gap between low-level operating system architecture and hardware execution.

## ⚙️ Current Architecture 
* **Process Control:** Implements manual process cloning using POSIX `fork()`.
* **Execution Engine:** Utilizes the `execvp()` system call to overwrite child memory maps with system binaries.
* **Memory Parsing:** Uses native C string manipulation (`strtok`) to tokenize user input directly in physical memory without heavy software abstractions.

## 🛠️ Usage
To compile and run the shell on a Unix system:
```bash
gcc main.c -o hsh
./hsh