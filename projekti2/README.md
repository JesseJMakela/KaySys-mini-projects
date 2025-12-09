# Unix Utilities Project (Project 2)

This project implements four simple UNIX utilities in C: `my-cat`, `my-grep`, `my-zip`, and `my-unzip`.

## Project Summary

This project recreates simplified versions of common UNIX command-line utilities to practice C programming, file I/O operations, and understanding of how UNIX utilities work internally. All programs compile with `-Wall -Werror` flags and handle errors appropriately.

## Implemented Utilities

### 1. my-cat

A simplified version of the `cat` utility that reads and displays file contents.

**Usage**: `./my-cat file1 [file2 ...]`

**Example**:

```bash
./my-cat test1.txt
```

### 2. my-grep

A simplified version of the `grep` utility that searches for text patterns in files.

**Usage**: `./my-grep searchterm [file ...]`

**Example**:

```bash
./my-grep foo test2.txt
echo "test foo" | ./my-grep foo
```

### 3. my-zip

A file compression utility using run-length encoding (RLE).

**Usage**: `./my-zip file1 [file2 ...] > output.z`

**Example**:

```bash
./my-zip test3.txt > compressed.z
```

### 4. my-unzip

A file decompression utility for files compressed with my-zip.

**Usage**: `./my-unzip file1.z [file2.z ...]`

**Example**:

```bash
./my-unzip compressed.z
./my-unzip compressed.z > original.txt
```

## Compilation

All utilities compile with strict warning flags:

```bash
gcc -o my-cat my-cat.c -Wall -Werror
gcc -o my-grep my-grep.c -Wall -Werror
gcc -o my-zip my-zip.c -Wall -Werror
gcc -o my-unzip my-unzip.c -Wall -Werror
```

Or use the provided Makefile:

```bash
make all
```

## Author

Jesse J Mäkelä

## Date

December 9, 2025
