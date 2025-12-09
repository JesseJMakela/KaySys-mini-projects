# Project 1: Warmup to C and Unix programming

This executable can reverse the words given in 3 different ways.

## Features

- three use cases
  - `./reverse` — read from stdin, write to stdout (exits using 0)
  - `./reverse input.txt` — read from `input.txt`, write to stdout
  - `./reverse input.txt output.txt` — read `input.txt`, write reversed lines to `output.txt`


## Build

Compile with gcc:

```bash
gcc -std=c11 -Wall -o reverse reverse.c
```

## Usage examples

1) Read from stdin and write to stdout

```bash
./reverse
hello
world
0

# Output:
# world
# hello
```

2) Read a file and print to screen

```bash
./reverse input.txt
```

3) Read a file and write to another file

```bash
./reverse input.txt output.txt
cat output.txt
```


## Makers details

- student: Jesse Mäkelä
- Email: Jesse.j.makela@student.lut.fi



