#ifndef __CHIP8_H__
#define __CHIP8_H__

#include "memory.h"
#include "stack.h"
#include "registers.h"

struct chip8 {
    byte *memory; 
    struct stack *stack;
    struct registers registers;
    int exit_code;   
};

#endif