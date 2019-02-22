#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "keyboard.h"

chip8_keyboard* create_keyboard() {
    chip8_keyboard* keyboard = malloc(sizeof(chip8_keyboard));

    memset(keyboard->keys, 0, KEYBOARD_KEYS_COUNT * sizeof(chip8_key_t));

    return keyboard;
}

void delete_keyboard(chip8_keyboard *keyboard) {
    free(keyboard);
}

void do_key_down(chip8_keyboard *keyboard, int key) {
    if (key < 0 || key >= KEYBOARD_KEYS_COUNT) {
        printf("down::Invalid key number: %d\n", key);
        abort();
    }

    keyboard->keys[key] = 1;
}

void do_key_up(chip8_keyboard *keyboard, int key) {
    if (key < 0 || key >= KEYBOARD_KEYS_COUNT) {
        printf("up::Invalid key number: %d\n", key);
        abort();
    }

    keyboard->keys[key] = 0;
}

int is_key_pressed(chip8_keyboard *keyboard, int key) {
    if (key < 0 || key >= KEYBOARD_KEYS_COUNT) {
        printf("is_pressed::Invalid key number: %d\n", key);
        abort();
    }

    return keyboard->keys[key];
}

int get_first_pressed(chip8_keyboard *keyboard) {
    for (int i = 0; i < KEYBOARD_KEYS_COUNT; i++) {
        if (keyboard->keys[i]) {
            return i;
        }
    }

    return -1;
}

int is_any_key_pressed(chip8_keyboard *keyboard) {
    for (int i = 0; i < KEYBOARD_KEYS_COUNT; i++) {
        if (keyboard->keys[i]) {
            return 1;
        }
    }

    return 0;
}