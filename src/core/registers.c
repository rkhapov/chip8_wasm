#include <stdlib.h>
#include <string.h>

#include "core/registers.h"

chip8_registers* create_registers() {
    chip8_registers *registers = malloc(sizeof(chip8_registers));
    
    registers->address = 0;
    registers->pc = 0;
    registers->v = malloc(sizeof(byte) * V_REGISTERS_AMOUNT);

    memset(registers->v, 0, sizeof(byte) * V_REGISTERS_AMOUNT);

    return registers;
}

void delete_registers(chip8_registers *registers) {
    free(registers->v);
    free(registers);
}