#ifndef __CHIP8_MEMORY_H__
#define __CHIP8_MEMORY_H__

#include "chip8_types.h"

typedef struct {
    chip8_memory_cell_t *memory;
    int size;
} chip8_memory;

chip8_memory* create_memory(int size);
void delete_memory(chip8_memory *memory);

#endif