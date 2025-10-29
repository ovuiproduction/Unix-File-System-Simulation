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
# Available commands (usage)
man                      # Display help
ls [-l]                  # List files/directories, -l for details (date/type)
pwd                      # Print working directory
cd <dir>                 # Change directory (.., ~ or / supported)
mkdir <dir>              # Create a new directory
touch <file>             # Create a new empty file
rm <file/dir>            # Remove a file or empty directory
cp <src> <dst>           # Copy file (files only)
mv <src> <dst_dir>       # Move file or directory into destination directory
rename <old> <new>       # Rename file or directory in current dir
cat <file>               # Display file content
echo > <file>            # Write a single line to file (overwrites or creates)
find <name>              # Find file/directory path (searches entire tree)
tree                     # Display directory tree for current directory
clear                    # Clear the screen
exit                     # Exit the program
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
git clone https://github.com/ovuiproduction/Unix-File-System-Simulation.git
cd Unix-File-System-Simulation
gcc Unix_File_System_Simulation.c -o app.exe
./app.exe
```
