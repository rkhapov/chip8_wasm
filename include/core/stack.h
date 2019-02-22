#ifndef __CHIP8_STACK__H__
#define __CHIP8_STACK__H__

#include "chip8_types.h"

struct stack {
    word *values;
    int size;
    int top;
};

struct stack* create_stack(int size);
void delete_stack(struct stack* stack);
void push(struct stack* stack, word value);
word peek(struct stack* stack);
word pop(struct stack* stack);
int is_full(struct stack* stack);
int is_empty(struct stack* stack);

#endif