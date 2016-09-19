// C Befunge Interpreter
// Author: xxc3nsoredxx

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char **argv) {
    char *filename; 
    if (argc < 2) {
        printf ("Please include a file name\n");
        return -1;
    } else {
        filename = (char*) malloc (strlen (*(argv + 1)));
        filename = *(argv + 1);
    }

    FILE *in = fopen (filename, "r");
    if (!in) {
        printf ("File not found: %s\n", filename);
        free (filename);
        return -1;
    }

    int num_lines = 1, num_cols = 1;
    int pos_row = 0, pos_col = 0;
    char **instruction_space = (char**) malloc (sizeof (char*));
    *(instruction_space + 0) = (char*) calloc (1, 1);

    char current_read = 0;

    while ((current_read = fgetc (in)) != EOF) {
        if (current_read == '\n') {
            num_lines++;
            printf ("%i\n", num_lines);
            instruction_space = (char**) realloc (instruction_space, sizeof (char*) * num_lines);
            *(instruction_space + (num_lines - 1)) = (char*) calloc (num_cols, 1);
            pos_row++;
            pos_col = 0;
            continue;
        }

        if (pos_col == num_cols) {
            num_cols++;
            for (int row = 0; row < num_lines; row++) {
                *(instruction_space + row) = (char*) realloc (*(instruction_space + row), num_cols);
            }
        }

        *(*(instruction_space + pos_row) + pos_col) = current_read;
        pos_col++;
    }
    
    for (int r = 0; r < num_lines; r++) {
        for (int c = 0; c < num_cols; c++) {
            printf ("%c", *(*(instruction_space + r) + c));
        }
        printf ("\n");
    }

    fclose (in);

    free (filename);
    for (int cx = 0; cx < num_lines; cx++) {
        free (*(instruction_space + cx));
    }
    free (instruction_space);

    return 0;
}
