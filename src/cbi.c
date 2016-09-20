// C Befunge Interpreter
// Author: xxc3nsoredxx

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAKE_PRINT(X) X = (X < 0x20) ? 0x20 : X

int main (int argc, char **argv) {
    // Load file
    char *filename; 
    // File not in args
    if (argc < 2) {
        printf ("Please include a file name\n");
        return -1;
    } else {
        filename = (char*) malloc (strlen (*(argv + 1)));
        filename = *(argv + 1);
    }

    FILE *in = fopen (filename, "r");
    // File not found
    if (!in) {
        printf ("File not found: %s\n", filename);
        free (filename);
        return -1;
    }

    // Initialize the instruction space to be a single spot
    int num_lines = 1, num_cols = 1;
    int pos_row = 0, pos_col = 0;
    char **instruction_space = (char**) malloc (sizeof (char*));
    *(instruction_space + 0) = (char*) calloc (1, 1);

    // Each char is read into here
    // Used for testing the char
    char current_read = 0;

    // Reads until the end of the file
    while ((current_read = fgetc (in)) != EOF) {
        // If we have a line longer than the previous max, extend them all out one
        if (pos_col == num_cols) {
            num_cols++;
            for (int row = 0; row < num_lines; row++) {
                *(instruction_space + row) = (char*) realloc (*(instruction_space + row), num_cols);
            }
        }

        // Add the instruction into the instruction space
        *(*(instruction_space + pos_row) + pos_col) = current_read;
        pos_col++;

        // If there is a newline, add a new row
        if (current_read == '\n') {
            num_lines++;
            //printf ("%i\n", num_lines);
            instruction_space = (char**) realloc (instruction_space, sizeof (char*) * num_lines);
            *(instruction_space + (num_lines - 1)) = (char*) calloc (num_cols, 1);
            pos_row++;
            pos_col = 0;
        }
    }

    // Make nonprintables whitespace
    for (int r = 0; r < num_lines; r++) {
        for (int c = 0; c < num_cols; c++) {
            MAKE_PRINT(*(*(instruction_space + r) + c));
            //printf ("0x%02X ", *(*(instruction_space + r) + c));
        }
        printf ("\n");
    }

    // Release everything from memory
    fclose (in);

    free (filename);
    for (int cx = 0; cx < num_lines; cx++) {
        free (*(instruction_space + cx));
    }
    free (instruction_space);

    return 0;
}
