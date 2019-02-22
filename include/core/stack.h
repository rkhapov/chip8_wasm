#ifndef __CHIP8_STACK__H__
#define __CHIP8_STACK__H__

#include "chip8_types.h"

typedef struct {
    word *values;
    int size;
    int top;
} chip8_stack;

chip8_stack* create_stack(int size);
void delete_stack(chip8_stack* stack);
void push(chip8_stack* stack, word value);
word peek(chip8_stack* stack);
word pop(chip8_stack* stack);
int is_full(chip8_stack* stack);
int is_empty(chip8_stack* stack);

#endif