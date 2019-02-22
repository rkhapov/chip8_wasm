#ifndef __CHIP8_REGISTERS_H__
#define __CHIP8_REGISTERS_H__

#include <memory.h>

#include "chip8_types.h"
#include "constants.h"

struct registers {
    word pc;
    word address;
    byte v[V_REGISTERS_AMOUNT]; 
};

struct registers create_registers();

#endif