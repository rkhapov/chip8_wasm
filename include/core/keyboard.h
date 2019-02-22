#ifndef __CHIP8_KEYBOARD_H__
#define __CHIP8_KEYBOARD_H__

#include "constants.h"

typedef int chip8_key_t;

typedef struct {
    chip8_key_t keys[KEYBOARD_KEYS_COUNT];    
} chip8_keyboard;

chip8_keyboard* create_keyboard();
void delete_keyboard(chip8_keyboard *keyboard);
void do_key_down(chip8_keyboard *keyboard, int key);
void do_key_up(chip8_keyboard *keyboard, int key);
int is_key_pressed(chip8_keyboard *keyboard, int key);
int get_first_pressed(chip8_keyboard *keyboard);
int is_any_key_pressed(chip8_keyboard *keyboard);

#endif