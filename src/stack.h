#ifndef STACK_H_20160919_205120
#define STACK_H_20160919_205120

typedef enum bool_e {
    true, false
} bool;

typedef struct stack_s {
    char instruction;
    struct stack_s *next;
} stack_t;

stack_t* init ();
bool push (stack_t*, char);
char pop (stack_t*);
bool clear (stack_t*);

#endif
