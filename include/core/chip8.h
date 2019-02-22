#ifndef __CHIP8_H__
#define __CHIP8_H__

#include "memory.h"
#include "stack.h"
#include "registers.h"
#include "screen.h"
#include "keyboard.h"

typedef struct {
    byte *memory; 
    chip8_stack *stack;
    chip8_screen *screen;
    chip8_keyboard *keyboard;
    
    chip8_registers registers;
    int exit_code;   
} chip8;

chip8* create_chip8();
void delete_chip8(chip8* chip8);

int finished(chip8* chip8);

#endif