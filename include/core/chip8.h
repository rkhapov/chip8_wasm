#ifndef __CHIP8_H__
#define __CHIP8_H__

#include "memory.h"
#include "stack.h"
#include "registers.h"
#include "screen.h"
#include "keyboard.h"

typedef struct {
    chip8_memory *memory; 
    chip8_stack *stack;
    chip8_screen *screen;
    chip8_keyboard *keyboard;    
    chip8_registers *registers;

    int delay_timer;
    int sound_timer;

    // chip8 non-standard objects
    int exit_code;   
    // if blocked, PC are not increased after instruction execution
    int blocked;
} chip8;

chip8* create_chip8();
void delete_chip8(chip8* chip8);
void reset(chip8 *chip8);
int finished(chip8* chip8);
void load_program(chip8 *chip8, byte *program, int size, int offset);
int load_program_file(chip8 *chip8, const char *path, int offset);
void execute_next(chip8 *chip8);
int end_program_reached(chip8 *chip8);
word get_next_opcode(chip8 *chip8);
void tick_timers(chip8 *chip8);

#endif