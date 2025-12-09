/*
 * my-grep.c - A simple implementation of the grep utility
 *
 * This program searches for a pattern in one or more files and prints matching lines.
 * Usage: ./my-grep searchterm [file ...]
 *
 * Exit codes:
 *   0 - Success
 *   1 - Error (no searchterm provided or cannot open file)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_BUFFER_SIZE 128

/*
 * Read a line from file stream with dynamic buffer allocation
 * Returns the line or NULL on EOF or error
 */
char *read_line(FILE *fp)
{
    size_t buffer_size = INITIAL_BUFFER_SIZE;
    size_t position = 0;
    char *buffer = malloc(buffer_size);

    if (buffer == NULL)
    {
        return NULL;
    }

    while (1)
    {
        int c = fgetc(fp);

        // End of file or error
        if (c == EOF)
        {
            if (position == 0)
            {
                free(buffer);
                return NULL;
            }
            buffer[position] = '\0';
            return buffer;
        }

        // Store character
        buffer[position++] = c;

        // End of line
        if (c == '\n')
        {
            buffer[position] = '\0';
            return buffer;
        }

        // Expand buffer if needed
        if (position >= buffer_size - 1)
        {
            buffer_size *= 2;
            char *new_buffer = realloc(buffer, buffer_size);
            if (new_buffer == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = new_buffer;
        }
    }
}

/*
 * Process a file and print lines containing the search term
 */
void grep_file(const char *searchterm, FILE *fp)
{
    char *line;

    while ((line = read_line(fp)) != NULL)
    {
        // Check if searchterm is in the line
        if (strstr(line, searchterm) != NULL)
        {
            printf("%s", line);
        }
        free(line);
    }
}

int main(int argc, char *argv[])
{
    // Check for correct usage
    if (argc < 2)
    {
        printf("my-grep: searchterm [file ...]\n");
        exit(1);
    }

    const char *searchterm = argv[1];

    // If no files specified, read from stdin
    if (argc == 2)
    {
        grep_file(searchterm, stdin);
        return 0;
    }

    // Process each file argument
    for (int i = 2; i < argc; i++)
    {
        FILE *fp = fopen(argv[i], "r");

        // Check if file opened successfully
        if (fp == NULL)
        {
            printf("my-grep: cannot open file\n");
            exit(1);
        }

        grep_file(searchterm, fp);
        fclose(fp);
    }

    return 0;
}
