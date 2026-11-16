# 🐧 Linux System Programming in C

> **A comprehensive, hands-on exploration of Linux system-level programming — from process creation to lock-free concurrency — built entirely in C.**

[![C](https://img.shields.io/badge/Language-C23-blue.svg)](https://en.wikipedia.org/wiki/C23)
[![Platform](https://img.shields.io/badge/Platform-Linux-green.svg)](https://www.kernel.org/)
[![Build](https://img.shields.io/badge/Build-CMake%203.20%2B-orange.svg)](https://cmake.org/)
[![Status](https://img.shields.io/badge/Status-🚧%20Work%20in%20Progress-yellow.svg)]()

---

## 📖 About This Project

This repository is a **systematic, chapter-by-chapter deep dive** into Linux system programming using the C language. Each chapter focuses on a core operating-system concept and contains **self-contained, runnable C programs** that demonstrate the theory in practice.

The goal is to build **intuitive, low-level understanding** of how the Linux kernel manages processes, handles signals, performs I/O, enables inter-process communication, and coordinates concurrent threads — the exact skills that separate application developers from systems engineers.

**This project is actively under development.** New chapters, exercises, and refinements are being added regularly. Every chapter completed and every exercise implemented reflects a **structured, hands-on approach to mastering OS internals** — from process creation and signal handling to lock-free concurrency.

---

## 🎯 What You'll Learn

| Domain | Key Concepts |
|---|---|
| **C Fundamentals** | Pointers, virtual memory, dynamic allocation, pass-by-reference |
| **Process Management** | `fork()`, `vfork()`, orphan/zombie processes, memory layout |
| **Process Synchronization** | `wait()`, `waitpid()`, exit status handling |
| **Program Execution** | Full `exec` family (`execl`, `execv`, `execvp`, …), shell implementation |
| **Signal Handling** | `kill()`, `signal()`, `sigaction()`, signal masking, blocking, pending signals |
| **File I/O** | File descriptors, `open/read/write`, blocking vs non-blocking, `lseek`, `pread/pwrite`, `fsync/fdatasync`, `select()` multiplexing |
| **Inter-Process Communication** | Anonymous pipes, named pipes (FIFO), System V message queues, shared memory |
| **Threading & Concurrency** | POSIX threads, mutexes, read-write locks, spinlocks, semaphores, condition variables, barriers, atomic operations |

---

## 📂 Repository Structure

```
Linux-System-Programming/
│
├── chapter00_basics-of-c/                    # C language foundations
│   └── concept01_pointers/                   # 9 exercises
│       ├── Virtual memory & address spaces
│       ├── Pointer syntax & arithmetic
│       ├── Arrays and pointers
│       ├── Pass-by-value vs pass-by-reference
│       ├── Double pointers
│       └── Dynamic memory allocation
│
├── chapter01_fork-c/                         # Process creation — 7 exercises
│   ├── fork() syntax and behavior
│   ├── Parent/child variable scope & memory isolation
│   ├── Shared memory layout (read-only & write cases)
│   ├── Orphan process creation
│   ├── Zombie process creation
│   └── vfork() semantics
│
├── chapter02_wait-c/                         # Process synchronization — 3 exercises
│   ├── wait() syntax and exit status
│   ├── Waiting for multiple child processes
│   └── waitpid() with options
│
├── chapter03_exec-c/                         # Program execution — 7 exercises
│   ├── exec() family overview
│   ├── execlp(), execv(), execvp() variants
│   ├── Wrapper shell implementation
│   └── Terminal shell command simulation
│
├── chapter04_signal-c/                       # Signal handling — 12 exercises
│   ├── kill() and signal sending
│   ├── signal() for behavior alteration
│   ├── sigaction() for robust signal handling
│   ├── Signal blocking with sigaction
│   ├── Signal masking without sigaction
│   └── Disposition and mask signal actions
│
├── chapter05_file_descriptor/                # File I/O — 30 exercises
│   ├── open() system call and flags
│   ├── read() and write() system calls
│   ├── Blocking vs non-blocking I/O
│   ├── EINTR handling and recovery
│   ├── fsync(), fdatasync(), sync() — data persistence
│   ├── O_SYNC, O_DSYNC, O_RSYNC flags
│   ├── lseek() and file positioning
│   ├── Sparse files (seeking past EOF)
│   ├── pread() / pwrite() — positional I/O
│   ├── truncate() / ftruncate() — file sizing
│   ├── select() — multiplexed I/O
│   └── Portable sleep using select()
│
├── chapter06_ipc_mechanisms/                 # Inter-Process Communication — 9 exercises
│   ├── Anonymous pipes (parent-child communication)
│   ├── Named pipes (FIFO) — client/server model
│   ├── System V message queues — sender/receiver
│   ├── Message queues with message types — producer/consumer
│   └── Shared memory — multi-user access
│
├── chapter07_threads/                        # Threading & Concurrency — 22 exercises
│   │
│   ├── Part-1_Threading (Single Thread Management)    # 6 exercises
│   │   ├── Thread creation and lifecycle
│   │   ├── Passing arguments to threads
│   │   ├── Thread ID management
│   │   ├── Returning data from threads
│   │   ├── Detached threads
│   │   └── Thread cancellation & cleanup handlers
│   │
│   └── Part-2_Multi-Threading (Concurrency and Coordination)  # 16 exercises
│       │
│       ├── Topic-01_mutual_exclusion_paradigm          # 7 exercises
│       │   ├── Race condition demonstration
│       │   ├── Standard mutex (pthread_mutex_t)
│       │   ├── Mutex for shared group variables
│       │   ├── Read-write locks (reader-writer problem)
│       │   └── Spinlocks and contention analysis
│       │
│       ├── Topic-02_signaling_and_coordination         # 4 exercises
│       │   ├── Binary semaphores for thread signaling
│       │   ├── Condition variables
│       │   └── Counting semaphores for resource limiting
│       │
│       └── Topic-03_lock-free_hardware_synchronization # 2 exercises
│           └── Atomic operations (stdatomic.h, atomic_flag)
│
├── CMakeLists.txt                            # Auto-build configuration (C23, pthreads, atomics)
├── helloWorld.c                              # Quick sanity-check program
└── README.md                                 # This file
```

---

## 🛠️ Build & Run

### Prerequisites

- **Linux** (Ubuntu, Fedora, Arch, etc.) or WSL2
- **CMake** ≥ 3.20
- **GCC** or **Clang** with C23 support
- **pthread** and **libatomic** (usually pre-installed)

### Quick Start

```bash
# Clone the repository
git clone https://github.com/darkdevil03/Linux-System-Programming.git
cd Linux-System-Programming

# Build all exercises
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run any exercise (executables are in the build directory)
./Ex01_fork_syntax
./Ex01_pipes
./Ex01_thread_implementation
```

> **Note:** The `CMakeLists.txt` automatically discovers all `.c` files and builds each as a standalone executable linked against `pthread` and `atomic`. No manual target configuration needed.

---

## 📊 Project Statistics

| Metric | Value |
|---|---|
| **Chapters** | 8 |
| **Total Exercises** | ~99 |
| **C Standard** | C23 |
| **Libraries Used** | pthread, stdatomic, POSIX syscalls |
| **Topics Covered** | 7 major OS domains |

---

## 🔮 Upcoming Chapters (Planned)

The following topics are planned for future chapters:

- **Chapter 08 — Memory Management**: `mmap()`, `mprotect()`, memory-mapped files, virtual memory allocation
- **Chapter 09 — Advanced IPC**: POSIX shared memory, sockets (Unix domain & Internet), memory-mapped IPC
- **Chapter 10 — System Calls Internals**: Syscall tracing with `strace`, VDSO, syscall numbers, context switching
- **Chapter 11 — File Systems**: Inodes, directory entries, `stat()`, hard/soft links, file permissions
- **Chapter 12 — Timers & Scheduling**: `setitimer()`, `timer_create()`, `nanosleep()`, CPU scheduling policies
- **Chapter 13 — Advanced Threading**: Thread pools, work queues, lock-free data structures, memory models

---

## 💡 Why This Project Matters

This repository is more than a collection of C programs. It represents a **deliberate journey from C fundamentals to lock-free concurrency**, with every concept reinforced through hands-on implementation:

- **Deep understanding of OS internals** — not just API calls, but the *why* behind process isolation, signal delivery semantics, and memory consistency models.
- **Progressive complexity** — starting from basic pointers and building up to lock-free atomic operations, following a structured learning path.
- **Practical implementation skills** — every concept is backed by runnable code, not just theory. Exercises like building a shell, implementing producer-consumer patterns, and handling race conditions mirror real-world systems programming tasks.
- **Commitment to craft** — the project uses modern C (C23), clean build systems (CMake), and organized structure, reflecting professional engineering habits.

Each exercise is designed to be small enough to understand in isolation but connected enough to build a complete mental model of Linux system programming.

---

## 📝 License

This project is open-source and available for learning purposes.

---

## 🤝 Contributing

Suggestions, corrections, and additional exercises are welcome. Feel free to open an issue or submit a pull request.

---

> *"The only way to learn a new programming language is by writing programs in it."* — Ken Thompson

**⭐ If you find this project helpful, consider giving it a star — it motivates continued development!**
