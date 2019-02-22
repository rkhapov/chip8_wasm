#include <stdlib.h>

#include "chip8.h"

chip8* create_chip8() {
    chip8* c8 = malloc(sizeof(chip8));

    c8->exit_code = -1;
    c8->registers = create_registers();
    c8->memory = create_memory(RECOMMENDED_MEMORY_SIZE);
    c8->stack = create_stack(RECOMMENDED_STACK_SIZE);
    c8->screen = create_screen();
    c8->keyboard = create_keyboard();

    return c8;
}

void delete_chip8(chip8 *chip8) {
    delete_memory(chip8->memory);
    delete_stack(chip8->stack);
    delete_screen(chip8->screen);
    delete_keyboard(chip8->keyboard);

    free(chip8);
}

int finished(chip8 *chip8) {
    return chip8->exit_code != -1;
}