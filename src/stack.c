// Stack Implementation
// Author: xxc3nsoredxx

#include <stdlib.h>
#include "stack.h"

#define EMPTY_ENTRY ((stack_t*) malloc (sizeof (stack_t)))

// Initialize a stack
stack_t* init () {
    stack_t *ret = EMPTY_ENTRY;
    ret->value = -1;
    ret->next = 0;

    return ret;
}

// Push value onto stack
bool push (stack_t *s, const char value) {
    if (s->next == 0) {
        s->value = value;
        s->next = s;
        return true;
    }

    stack_t *new_s = EMPTY_ENTRY;
    new_s->value = value;
    new_s->next = s;
    s = new_s;

    return true;
}

// Pop value from stack
char pop (stack_t *s) {
    char ret = s->value;
    
    // If the current top of the stack is the only member, reset values
    // Else set the top of the stack to the next one and free the old top
    if (s->next == s) {
        s->value = -1;
        s->next = 0;
    } else {
        stack_t *del = s;
        s = s->next;
        free (del);
    }

    return ret;
}

// Flush the stack
bool clear (stack_t *s) {
    while (s->next != 0) {
        pop (s);
    }

    return true;
}
