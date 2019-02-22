#include <string.h>

#include "registers.h"

chip8_registers create_registers() {
    chip8_registers registers;
    
    registers.address = 0;
    registers.pc = 0;
    memset(registers.v, 0, sizeof(byte) * V_REGISTERS_AMOUNT);

    return registers;
}