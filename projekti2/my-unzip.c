/*
 * my-unzip.c - A simple run-length encoding (RLE) decompression utility
 *
 * This program decompresses files that were compressed with my-zip.
 * The input format is: 4-byte integer (run length) + 1 ASCII character
 * Usage: ./my-unzip compressed_file1 [compressed_file2 ...]
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
        printf("my-unzip: file1 [file2 ...]\n");
        exit(1);
    }

    // Process each file argument
    for (int i = 1; i < argc; i++)
    {
        FILE *fp = fopen(argv[i], "r");

        // Check if file opened successfully
        if (fp == NULL)
        {
            printf("my-unzip: cannot open file\n");
            exit(1);
        }

        // Read and decompress file
        int count;
        char character;

        // Read pairs of (count, character) until end of file
        while (fread(&count, sizeof(int), 1, fp) == 1)
        {
            if (fread(&character, sizeof(char), 1, fp) == 1)
            {
                // Print the character 'count' times
                for (int j = 0; j < count; j++)
                {
                    printf("%c", character);
                }
            }
        }

        fclose(fp);
    }

    return 0;
}
