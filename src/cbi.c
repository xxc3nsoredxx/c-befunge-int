// C Befunge Interpreter
// Author: xxc3nsoredxx

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

#define MAKE_PRINT(X) X = (X < 0x20) ? 0x20 : X
#define NUM(X) (((X) <= 0x39) ? (X) - 0x30 : 10 + (X) - 0x61)

typedef enum delta_e {
    LEFT, RIGHT, UP, DOWN
} delta_t;

int parse_command (char command, stack_t *stack, delta_t *delta) {
    (void) delta;
    int a = 0;
    switch (command) {
        case '+':
            a += pop (stack);
            a += pop (stack);
            push (stack, a);
            return 0;
        case '-':
            a -= pop (stack);
            a += pop (stack);
            push (stack, a);
            return 0;
        case '*':
            a += pop (stack);
            a *= pop (stack);
            push (stack, a);
            return 0;
        case '/':
            a += pop (stack);
            if (a == 0) {
                printf ("Error: division by zero.\n");
                return 1;
            }
            a = pop (stack) / a;
            push (stack, a);
            return 0;
        case '.':
            printf ("%i", pop (stack));
            return 0;
        case ',':
            printf ("%c", (char) pop (stack));
            return 1;
        case '@':
            return 1;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            push (stack, NUM(command));
            return 0;
        default:
            return 0;
    }
}

int main (int argc, char **argv) {
    char *filename; 
    FILE *in;
    int num_rows, num_cols;
    int pos_row, pos_col;
    char **instruction_space;
    char current_read;
    stack_t *stack;
    delta_t delta;

    // Load file
    // File not in args
    if (argc < 2) {
        printf ("Please include a file name\n");
        return -1;
    } else {
        filename = (char*) malloc (strlen (*(argv + 1)));
        filename = *(argv + 1);
    }

    in = fopen (filename, "r");
    // File not found
    if (!in) {
        printf ("File not found: %s\n", filename);
        free (filename);
        return -1;
    }

    // Initialize the instruction space to be a single spot
    num_rows = 1;
    num_cols = 1;
    pos_row = 0;
    pos_col = 0;
    instruction_space = (char**) malloc (sizeof (char*));
    *(instruction_space + 0) = (char*) calloc (num_cols, 1);

    // Each char is read into here
    // Used for testing the char
    current_read = 0;

    // Reads until the end of the file
    while ((current_read = fgetc (in)) != EOF) {
        // If we have a line longer than the previous max, extend them all out one
        if (pos_col == num_cols) {
            num_cols++;
            for (int row = 0; row < num_rows; row++) {
                *(instruction_space + row) = (char*) realloc (*(instruction_space + row), num_cols);
            }
        }

        // Add the instruction into the instruction space
        *(*(instruction_space + pos_row) + pos_col) = current_read;
        pos_col++;

        // If there is a newline, add a new row
        if (current_read == '\n') {
            num_rows++;
            //printf ("%i\n", num_rows);
            instruction_space = (char**) realloc (instruction_space, sizeof (char*) * num_rows);
            *(instruction_space + (num_rows - 1)) = (char*) calloc (num_cols, 1);
            pos_row++;
            pos_col = 0;
        }
    }

    // Make nonprintables whitespace
    for (int r = 0; r < num_rows; r++) {
        for (int c = 0; c < num_cols; c++) {
            MAKE_PRINT(*(*(instruction_space + r) + c));
            //printf ("0x%02X ", *(*(instruction_space + r) + c));
        }
        //printf ("\n");
    }

    // Initialize stack
    stack = init ();

    // Set up the delta
    delta = RIGHT;
    pos_row = 0;
    pos_col = 0;

    // Run through instruction space
    while (!(parse_command (*(*(instruction_space + pos_row) + pos_col), stack, &delta))) {
        switch (delta) {
            case LEFT:
                pos_col--;
                if (pos_col < 0) pos_col = num_cols - 1;
                break;
            case RIGHT:
                pos_col++;
                if (pos_col == num_cols) pos_col = 0;
                break;
            case UP:
                pos_row--;
                if (pos_row < 0) pos_row = num_rows - 1;
                break;
            case DOWN:
                pos_row++;
                if (pos_row == num_rows) pos_row = 0;
                break;
        }
    }

    // Release everything from memory
    fclose (in);

    free (filename);
    for (int cx = 0; cx < num_rows; cx++) {
        free (*(instruction_space + cx));
    }
    free (instruction_space);

    clear (stack);
    free (stack);

    return 0;
}
