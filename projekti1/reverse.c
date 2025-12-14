#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

/*
 * reverse: read lines from input (stdin or file) and print them in reverse
 * Supports: ./reverse
 *           ./reverse input.txt
 *           ./reverse input.txt output.txt
 *
 */

/* Check if two file paths refer to the same file using inode comparison */
int same_file(const char *a, const char *b)
{
    if (strcmp(a, b) == 0)
        return 1;

    struct stat sa, sb;
    if (stat(a, &sa) == 0 && stat(b, &sb) == 0)
    {
        if (sa.st_dev == sb.st_dev && sa.st_ino == sb.st_ino)
            return 1;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    FILE *fin = NULL;
    FILE *fout = NULL;
    int need_close_in = 0;
    int need_close_out = 0;

    /* Validate number of arguments */
    if (argc > 3)
    {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    /* Mode 1: Read from stdin, write to stdout */
    if (argc == 1)
    {
        fin = stdin;
        fout = stdout;
    }
    /* Mode 2: Read from file, write to stdout */
    else if (argc == 2)
    {
        fin = fopen(argv[1], "r");
        if (fin == NULL)
        {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        need_close_in = 1;
        fout = stdout;
    }
    /* Mode 3: Read from input file, write to output file */
    else
    {

        if (same_file(argv[1], argv[2]))
        {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }

        fin = fopen(argv[1], "r");
        if (fin == NULL)
        {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
        need_close_in = 1;

        fout = fopen(argv[2], "w");
        if (fout == NULL)
        {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            if (need_close_in)
                fclose(fin);
            exit(1);
        }
        need_close_out = 1;
    }

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;

    /* Initialize dynamic array to store lines */
    size_t capacity = 128;
    size_t count = 0;
    char **lines = malloc(capacity * sizeof(char *));
    if (lines == NULL)
    {
        fprintf(stderr, "malloc failed\n");
        if (need_close_in)
            fclose(fin);
        if (need_close_out)
            fclose(fout);
        exit(1);
    }

    /* Read all lines from input */
    while ((linelen = getline(&line, &linecap, fin)) != -1)
    {
        if (linelen > 0 && line[linelen - 1] == '\n')
            line[linelen - 1] = '\0';

        /* Check if line is "0" - signals end of input from stdin */
        if (argc == 1 && strcmp(line, "0") == 0)
        {
            break;
        }

        char *copy = malloc(strlen(line) + 1);
        if (copy == NULL)
        {
            fprintf(stderr, "malloc failed\n");
            free(line);
            for (size_t i = 0; i < count; i++)
                free(lines[i]);
            free(lines);
            if (need_close_in)
                fclose(fin);
            if (need_close_out)
                fclose(fout);
            exit(1);
        }
        strcpy(copy, line);

        /* Expand array if needed */
        if (count == capacity)
        {
            capacity *= 2;
            char **tmp = realloc(lines, capacity * sizeof(char *));
            if (tmp == NULL)
            {
                fprintf(stderr, "malloc failed\n");
                free(copy);
                free(line);
                for (size_t i = 0; i < count; i++)
                    free(lines[i]);
                free(lines);
                if (need_close_in)
                    fclose(fin);
                if (need_close_out)
                    fclose(fout);
                exit(1);
            }
            lines = tmp;
        }
        lines[count++] = copy;
    }

    free(line);

    /* Print lines in reverse order and free memory */
    for (ssize_t i = (ssize_t)count - 1; i >= 0; i--)
    {
        fprintf(fout, "%s\n", lines[i]);
        free(lines[i]);
    }

    free(lines);

    /* Close files if opened */
    if (need_close_in)
        fclose(fin);
    if (need_close_out)
        fclose(fout);

    return 0;
}
