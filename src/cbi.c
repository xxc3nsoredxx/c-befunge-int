// C Befunge Interpreter
// Author: xxc3nsoredxx

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "delta.h"
#include "stack.h"

#define MAKE_PRINT(X) X = (X < 0x20) ? 0x20 : X
#define NUM(X) (((X) <= 0x39) ? (X) - 0x30 : 10 + (X) - 0x61)
#define ABS(X) (((X) < 0) ? (-(X)) : (X))

typedef enum ascii_mode_e {
    OFF, SINGLE, MULTI
} ascii_mode_t;

int parse_command (char command, stack_t *stack, delta_t *delta) {
    static ascii_mode_t ascii_mode = OFF;
    static int repeat = 1;
    static int skip_num = 0;
    static int toggle_exec = 0;
    static int jumping_back = 0;

    if (skip_num > 0) {
        skip_num--;
        return 0;
    }

    if (jumping_back) {
        reflect (delta);
        jumping_back = 0;
    }

    if (toggle_exec) {
        if (command == ';') toggle_exec = 0;
        return 0;
    }

    //printf ("%i %i\n", delta->delta_x, delta->delta_y);
    if (ascii_mode == SINGLE) {
        push (stack, (int) command);
        ascii_mode = OFF;
        return 0;
    }

    if (ascii_mode == MULTI) {
        if (command == '\"') {
            ascii_mode = OFF;
            return 0;
        }
        push (stack, (int) command);
        return 0;
    }

    int a = 0;
    for (int cx = 0; cx < repeat; cx++) {
        switch (command) {
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
                break;
            case '+':
                a = pop (stack);
                a += pop (stack);
                push (stack, a);
                break;
            case '-':
                a = pop (stack);
                a = -a;
                a += pop (stack);
                push (stack, a);
                break;
            case '*':
                a = pop (stack);
                a *= pop (stack);
                push (stack, a);
                break;
            case '/':
                a = pop (stack);
                if (a == 0) {
                    printf ("Error: division by zero.\n");
                    return 1;
                }
                a = pop (stack) / a;
                push (stack, a);
                break;
            case '\'':
                ascii_mode = SINGLE;
                break;
            case '\"':
                ascii_mode = MULTI;
                break;
            case '.':
                printf ("%i", pop (stack));
                break;
            case ',':
                printf ("%c", (char) pop (stack));
                break;
            case ':':
                a += pop (stack);
                push (stack, a);
                push (stack, a);
                break;
            case 'k':
                repeat = pop (stack);
                repeat += (repeat > 0) ? 1 : 0;
                return 0;
            case '^':
                up (delta);
                break;
            case 'v':
                down (delta);
                break;
            case '<':
                left (delta);
                break;
            case '>':
                right (delta);
                break;
            case '[':
                rel_left (delta);
                break;
            case ']':
                rel_right (delta);
                break;
            case 'r':
                reflect (delta);
                break;
            case '#':
                skip_num = 1;
                break;
            case 'j':
                skip_num = pop (stack);
                if (skip_num < 0) {
                    reflect (delta);
                    skip_num = ABS(skip_num);
                    skip_num -= 2;
                }
                break;
            case ';':
                toggle_exec = 1;
                break;
            case '@':
                return 1;
            default:
                break;
        }
    }

    repeat = 1;

    return 0;
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
    delta.delta_x = 1;
    delta.delta_y = 0;
    pos_row = 0;
    pos_col = 0;

    // Run through instruction space
    while (!(parse_command (*(*(instruction_space + pos_row) + pos_col), stack, &delta))) {
        //printf ("%c\n", *(*(instruction_space + pos_row) + pos_col));
        pos_col += delta.delta_x;
        pos_row += delta.delta_y;
        if (pos_col < 0) pos_col = num_cols - 1;
        if (pos_col == num_cols) pos_col = 0;
        if (pos_row < 0) pos_row = num_rows - 1;
        if (pos_row == num_rows) pos_row = 0;
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
