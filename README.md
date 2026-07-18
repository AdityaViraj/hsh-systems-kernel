# '**hsh**': A Hardware Aware Shell
This is my attempt at building a shell completely from scratch in C — no shortcuts, no wrapping around system(). It sits between you and the OS and actually talks to the kernel directly for the stuff that matters.

How it is built?

At its heart hsh is just a REPL — Read, Evaluate, Print, Loop — an infinite loop that keeps reading whatever you type, chews it up, and hands it off to the OS. Sounds trivial until you realize "handing it off to the OS" means doing everything yourself instead of letting libc quietly do it for you.


Process Management: Every time you hit enter, I fork() a brand new child process and execvp() inside it to swap that child's memory out for whatever binary you asked for. The parent just sits there on waitpid() until the child's done, so nothing runs out of order.
Path resolution: When you type a bare command like ls, the shell has no clue where that lives — so I grab PATH with getenv(), split it apart, and walk every directory in it with access() until something matches. Basically doing by hand what your terminal normally hides from you.
Memory Management: Every token gets allocated as the input gets parsed, and every single command cycle cleans that memory back up before the next prompt — no leaks piling up after a thousand commands.
Hardware-level primitives (metal.h): This is the part I'm most proud of. Instead of calling the standard exit() and letting libc handle it, I hand-wrote raw syscalls for exit and cycle-counting — split by OS and CPU, because it turns out macOS and Linux don't even agree on what number "exit" is! Got that wrong once and lost an evening figuring out why the shell just... refused to exit on Linux.

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
