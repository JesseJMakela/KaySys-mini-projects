# Unix Utilities Project (Project 2)

This project implements four simple UNIX utilities in C: `my-cat`, `my-grep`, `my-zip`, and `my-unzip`.

**Repository**: https://github.com/JesseJMakela/reverse

## Project Summary

This project recreates simplified versions of common UNIX command-line utilities to practice C programming, file I/O operations, and understanding of how UNIX utilities work internally. All programs compile with `-Wall -Werror` flags and handle errors appropriately.

## Implemented Utilities

### 1. my-cat

A simplified version of the `cat` utility that reads and displays file contents.

**Usage**: `./my-cat file1 [file2 ...]`

**Features**:

- Reads one or more files and prints their contents to stdout
- Handles files with arbitrarily long lines
- Returns exit code 0 on success
- Returns exit code 1 and prints error message if file cannot be opened

**Example**:

```bash
./my-cat test1.txt
```

### 2. my-grep

A simplified version of the `grep` utility that searches for text patterns in files.

**Usage**: `./my-grep searchterm [file ...]`

**Features**:

- Searches for a specified string in one or more files
- Prints lines containing the search term
- Case-sensitive matching
- Can read from stdin if no files are specified
- Handles arbitrarily long lines using dynamic memory allocation
- Returns exit code 0 on success
- Returns exit code 1 if no search term provided or file cannot be opened

**Example**:

```bash
./my-grep foo test2.txt
echo "test foo" | ./my-grep foo
```

### 3. my-zip

A file compression utility using run-length encoding (RLE).

**Usage**: `./my-zip file1 [file2 ...] > output.z`

**Features**:

- Compresses files using run-length encoding
- Output format: 4-byte integer (count) + 1-byte character
- Multiple input files are compressed into a single output stream
- Writes compressed data to stdout (use shell redirection to save)
- Returns exit code 0 on success
- Returns exit code 1 if no files provided or file cannot be opened

**Example**:

```bash
./my-zip test3.txt > compressed.z
```

**Compression Details**:

- Consecutive identical characters are encoded as a count followed by the character
- Example: "aaaabbbb" becomes: `[4][a][4][b]` (in binary format)
- Works best with files containing many repeated characters

### 4. my-unzip

A file decompression utility for files compressed with my-zip.

**Usage**: `./my-unzip file1.z [file2.z ...]`

**Features**:

- Decompresses files created by my-zip
- Reads 4-byte integer + 1-byte character pairs
- Writes decompressed data to stdout
- Returns exit code 0 on success
- Returns exit code 1 if no files provided or file cannot be opened

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

## Testing

### Test my-cat

```bash
# Display a single file
./my-cat test1.txt

# Display multiple files
./my-cat test1.txt test2.txt

# Test with no arguments (should exit with code 0)
./my-cat

# Test with non-existent file (should print error and exit with code 1)
./my-cat nonexistent.txt
```

### Test my-grep

```bash
# Search for "foo" in a file
./my-grep foo test2.txt

# Search from stdin
echo "test foo bar" | ./my-grep foo

# Test with no arguments (should print usage and exit with code 1)
./my-grep

# Test with non-existent file (should print error and exit with code 1)
./my-grep foo nonexistent.txt
```

### Test my-zip and my-unzip

```bash
# Compress a file
./my-zip test3.txt > compressed.z

# Decompress a file
./my-unzip compressed.z

# Round-trip test (should match original)
./my-zip test3.txt > temp.z
./my-unzip temp.z > output.txt
diff test3.txt output.txt  # Should show no differences

# Test compression ratio
ls -lh test4.txt test4.z  # Compare sizes
```

## Test Files Included

- `test1.txt` - Simple multi-line text file for testing my-cat
- `test2.txt` - Text file with "foo" pattern for testing my-grep
- `test3.txt` - File with repeated characters ("aaaaaaaaaabbbb") for testing compression
- `test4.txt` - Larger file with many repeated characters to demonstrate compression efficiency

## Implementation Details

### my-cat.c

- Uses `fopen()`, `fgets()`, and `fclose()` for file operations
- Fixed buffer size (1024 bytes) per read
- Simple sequential file processing

### my-grep.c

- Implements dynamic line reading with `read_line()` function
- Uses `malloc()` and `realloc()` for handling arbitrarily long lines
- Uses `strstr()` for substring matching
- Properly frees allocated memory to prevent leaks

### my-zip.c

- Reads files character by character using `fgetc()`
- Tracks consecutive character runs
- Uses `fwrite()` to output binary data (4-byte int + 1-byte char)
- Processes multiple files as a single continuous stream

### my-unzip.c

- Uses `fread()` to read binary data in 5-byte chunks
- Reads 4-byte integer for count, then 1-byte character
- Outputs characters using `printf()` in a loop

## Error Handling

All utilities implement proper error handling:

- Check return values from file operations
- Print appropriate error messages to stdout
- Exit with code 1 on errors, 0 on success
- Handle edge cases (no arguments, missing files, etc.)

## Code Quality

- All code compiles without warnings with `-Wall -Werror`
- Includes detailed comments explaining functionality
- Follows consistent coding style
- Proper memory management (no leaks in my-grep)
- Handles edge cases appropriately

## Screenshots and Results

### my-cat Output

```
$ ./my-cat test1.txt
Hello World
This is a test file
It has multiple lines
```

### my-grep Output

```
$ ./my-grep foo test2.txt
foo bar
This line has foo in it
foolish test
```

### my-zip/my-unzip Compression

```
$ ls -lh test4.txt test4.z
-rw-r--r-- 1 user user 1.8K Dec  2 11:25 test4.txt
-rw-r--r-- 1 user user   20 Dec  2 11:25 test4.z

$ ./my-unzip test4.z | wc -c
1801
$ wc -c test4.txt
1801 test4.txt
```

This demonstrates ~99% compression ratio for files with long runs of repeated characters.

## Author

Jesse J Mäkelä

## Course

LUT University - Operating Systems Project 2

## Date

December 2, 2025
