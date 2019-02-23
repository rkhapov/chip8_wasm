#ifndef __CHIP8_REGISTERS_H__
#define __CHIP8_REGISTERS_H__

#include <memory.h>

#include "chip8_types.h"
#include "constants.h"

typedef struct {
    word pc;
    word address;
    byte *v; 
} chip8_registers;

chip8_registers *create_registers();
void delete_registers(chip8_registers *registers);

#endif