#ifndef __CHIP8_STACK__H__
#define __CHIP8_STACK__H__

#include <stdint.h>

struct stack {
    uint16_t *values;
    int size;
    int top;
};

struct stack* create_stack(int size);
void delete_stack(struct stack* stack);
void push(struct stack* stack, uint16_t value);
uint16_t peek(struct stack* stack);
uint16_t pop(struct stack* stack);
int is_full(struct stack* stack);
int is_empty(struct stack* stack);

#endif