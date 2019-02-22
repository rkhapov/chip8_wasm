#include <stdlib.h>

#include "chip8.h"

chip8* create_chip8() {
    chip8* c8 = malloc(sizeof(chip8));

    c8->exit_code = -1;
    c8->registers = create_registers();
    c8->memory = create_memory(RECOMMENDED_MEMORY_SIZE);
    c8->stack = create_stack(RECOMMENDED_STACK_SIZE);

    return c8;
}

void delete_chip8(chip8 *chip8) {
    delete_memory(chip8->memory);
    delete_stack(chip8->stack);
}