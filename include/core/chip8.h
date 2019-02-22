#ifndef __CHIP8_H__
#define __CHIP8_H__

#include "memory.h"
#include "stack.h"
#include "registers.h"

typedef struct {
    byte *memory; 
    chip8_stack *stack;
    registers registers;
    int exit_code;   
} chip8;

chip8* create_chip8();
void delete_chip8(chip8* chip8);

#endif