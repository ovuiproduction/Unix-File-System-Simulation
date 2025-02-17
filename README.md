## Unix File System Simulation using N-ary Tree

### Overview

This project simulates a Unix-like file system using an N-ary tree data structure. It provides essential file and directory operations such as creation, deletion, traversal, and renaming. The project also includes a command-line interface that mimics Unix shell commands.

### Tech Stack
```bash
# Programming Language
C

# Data Structures
N-ary Tree

# Concepts
File System Management
```

### File System Operations
```bash
# Create a new directory
mkdir <dir>

# Create a new file
touch <file>

# Change directory
cd <dir>

# List files and directories
ls

# Display the current path
pwd

# Delete a file or directory
rm <file/dir>

# Copy a file or directory
cp <src file> <dest file>

# Move or rename a file or directory
mv <src file> <dest file>

# Display file content
cat <file>

# Search for a file or directory
find <file>
```
### Features

1. File and Directory Management: Create, delete, rename, and move files and directories.

2. Navigation: Change directories (cd), list contents (ls), and print the working directory (pwd).

3. File Operations: Create files (touch), read content (cat), and copy files (cp).

4. Search: Find files and directories using the find command.

Mimics Unix Shell Commands: Supports basic Unix commands like mkdir, rm, mv, etc.

### Installation

Use the following commands to install and run the project:

```bash
git clone <repository_url>
cd unix-fs-simulation
gcc -o unix_fs_simulation main.c
./unix_fs_simulation
```
