# Filesystem Shell

## Overview

Filesystem Shell is an educational operating systems project developed in C for the Operating Systems course at Sapienza University of Rome.

The project implements a persistent Unix-like filesystem stored inside a memory-mapped file using the POSIX `mmap` interface. An interactive shell provides commands for creating, navigating and managing files and directories while preserving the filesystem state across multiple executions.

---

## Key Features

- Persistent filesystem based on memory-mapped files (`mmap`)
- Interactive command-line shell
- File and directory management
- Support for relative and absolute path navigation
- Persistent storage synchronization
- Modular implementation with separated filesystem and shell components

---

## Supported Commands

| Command | Description |
|----------|-------------|
| `format <file> <size>` | Creates a new filesystem |
| `mount <file>` | Mounts an existing filesystem |
| `mkdir <dir>` | Creates a new directory |
| `cd <dir>` | Changes the current directory (`..` and `/` supported) |
| `touch <file>` | Creates an empty file |
| `append <file> <text>` | Appends text to a file |
| `cat <file>` | Displays the contents of a file |
| `ls` | Lists files and directories |
| `rm <file/dir>` | Removes a file or an empty directory |
| `close` | Closes the mounted filesystem |

---

## Compilation

```bash
make
```

---

## Execution

```bash
./filesystem-shell
```

---

## Example Usage

```text
format disk.fs 1048576

mount disk.fs

mkdir documents

cd documents

touch notes.txt

append notes.txt Hello World

cat notes.txt

ls

cd /

close
```

---

## Technologies

- C
- POSIX API
- mmap
- msync
- munmap
- Makefile
- Linux / WSL

---

## Learning Objectives

This project was developed to gain practical experience with:

- Operating Systems concepts
- Persistent filesystem implementation
- Memory-mapped files (`mmap`)
- File and directory management
- POSIX system calls
- Modular software design in C

---

## Project Context

Academic project developed for the Operating Systems course within the Bachelor's Degree in Computer Engineering and Automation at Sapienza University of Rome.
