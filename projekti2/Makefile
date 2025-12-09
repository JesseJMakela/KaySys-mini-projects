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

# Run basic tests
test: all
	@echo "=== Testing my-cat ==="
	./my-cat test1.txt
	@echo ""
	@echo "=== Testing my-grep ==="
	./my-grep foo test2.txt
	@echo ""
	@echo "=== Testing my-zip/my-unzip ==="
	./my-zip test3.txt > test_output.z
	./my-unzip test_output.z
	@rm -f test_output.z
	@echo ""
	@echo "All tests completed!"

.PHONY: all clean test
