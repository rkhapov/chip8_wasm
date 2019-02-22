#ifndef __CHIP8_REGISTERS_H__
#define __CHIP8_REGISTERS_H__

#include <memory.h>

#include "chip8_types.h"
#include "constants.h"

typedef struct {
    word pc;
    word address;
    byte v[V_REGISTERS_AMOUNT]; 
} registers;

registers create_registers();

#endif