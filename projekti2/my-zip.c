/*
 * my-zip.c - A simple run-length encoding (RLE) compression utility
 *
 * This program compresses one or more files using run-length encoding.
 * The output format is: 4-byte integer (run length) + 1 ASCII character
 * Usage: ./my-zip file1 [file2 ...] > compressed_file
 *
 * Exit codes:
 *   0 - Success
 *   1 - Error (no files provided or cannot open file)
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Check for correct usage
    if (argc < 2)
    {
        printf("my-zip: file1 [file2 ...]\n");
        exit(1);
    }

    int current_char = -1; // Current character being counted (-1 means no character yet)
    int count = 0;         // Count of consecutive occurrences

    // Process each file argument
    for (int i = 1; i < argc; i++)
    {
        FILE *fp = fopen(argv[i], "r");

        // Check if file opened successfully
        if (fp == NULL)
        {
            printf("my-zip: cannot open file\n");
            exit(1);
        }

        // Read file character by character
        int c;
        while ((c = fgetc(fp)) != EOF)
        {
            if (current_char == -1)
            {
                // First character
                current_char = c;
                count = 1;
            }
            else if (c == current_char)
            {
                // Same character, increment count
                count++;
            }
            else
            {
                // Different character, write out the previous run
                fwrite(&count, sizeof(int), 1, stdout);
                fwrite(&current_char, sizeof(char), 1, stdout);

                // Start new run
                current_char = c;
                count = 1;
            }
        }

        fclose(fp);
    }

    // Write out the last run (if any)
    if (current_char != -1)
    {
        fwrite(&count, sizeof(int), 1, stdout);
        fwrite(&current_char, sizeof(char), 1, stdout);
    }

    return 0;
}
