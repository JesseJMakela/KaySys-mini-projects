# Makefile for Unix Utilities Project
# Compiles my-cat, my-grep, my-zip, and my-unzip

CC = gcc
CFLAGS = -Wall -Werror

# Target executables
TARGETS = my-cat my-grep my-zip my-unzip

# Default target: build all utilities
all: $(TARGETS)

# Individual targets
my-cat: my-cat.c
	$(CC) $(CFLAGS) -o my-cat my-cat.c

my-grep: my-grep.c
	$(CC) $(CFLAGS) -o my-grep my-grep.c

my-zip: my-zip.c
	$(CC) $(CFLAGS) -o my-zip my-zip.c

my-unzip: my-unzip.c
	$(CC) $(CFLAGS) -o my-unzip my-unzip.c

# Clean up compiled files
clean:
	rm -f $(TARGETS) *.z *.o

