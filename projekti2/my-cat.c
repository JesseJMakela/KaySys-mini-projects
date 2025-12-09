/*
 * my-cat.c - A simple implementation of the cat utility
 *
 * This program reads one or more files and prints their contents to stdout.
 * Usage: ./my-cat file1 [file2 ...]
 *
 * Exit codes:
 *   0 - Success
 *   1 - Error opening file
 */

#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    // If no files specified, just exit with success
    if (argc < 2)
    {
        return 0;
    }

    // Process each file argument
    for (int i = 1; i < argc; i++)
    {
        FILE *fp = fopen(argv[i], "r");

        // Check if file opened successfully
        if (fp == NULL)
        {
            printf("my-cat: cannot open file\n");
            exit(1);
        }

        // Read and print file contents line by line
        char buffer[BUFFER_SIZE];
        while (fgets(buffer, BUFFER_SIZE, fp) != NULL)
        {
            printf("%s", buffer);
        }

        // Close the file
        fclose(fp);
    }

    return 0;
}
