# Makefile for Unix Shell (wish)

CC = gcc
CFLAGS = -Wall -Werror
TARGET = wish
SRC = unix_shell.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) *.o *.txt output.txt file.txt test_redirect.txt
