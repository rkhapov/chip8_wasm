#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "chip8.h"

#define BUFFER_SIZE 4096

byte buffer[BUFFER_SIZE];

chip8* create_chip8() {
    chip8* c8 = malloc(sizeof(chip8));

    c8->exit_code = -1;
    c8->blocked = 0;
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
    delete_registers(chip8->registers);

    free(chip8);
}

int finished(chip8 *chip8) {
    return chip8->exit_code != -1;
}

void load_program(chip8 *chip8, byte *program, int size, int offset) {
    if (offset < 0 || offset + size >= chip8->memory->size) {
        return;
    }

    memcpy(chip8->memory + offset, program, sizeof(byte) * size);

    chip8->registers->pc = offset;
}

int load_program_file(chip8 *chip8, const char *path, int offset) {
    if (offset < 0) {
        return 0;
    }

    FILE *file = fopen(path, "rb");

    if (file == NULL) {
        return 0;
    }

    int memory_offset = offset;

    while (1) {
        size_t read = fread(buffer, sizeof(byte), BUFFER_SIZE, file);

        if (read * sizeof(byte) + memory_offset >= chip8->memory->size) {
            fclose(file);
            return 0;
        }

        memcpy(chip8->memory + memory_offset, buffer, read);
        memory_offset += read * sizeof(byte);

        if (read < sizeof(byte) * BUFFER_SIZE) {
            break;
        }
    }

    fclose(file);

    return 1;
}