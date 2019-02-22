#ifndef __CHIP8_SCREEN_H__
#define __CHIP8_SCREEN_H__

#include "chip8_types.h"
#include "constants.h"

typedef int chip8_pixel_t;

typedef struct {
    chip8_pixel_t **pixels;
} chip8_screen;

chip8_screen* create_screen();
void delete_screen(chip8_screen *screen);
void clear_screen(chip8_screen *screen);
chip8_pixel_t set_pixel(chip8_screen *screen, int y, int x, chip8_pixel_t value);
chip8_pixel_t get_pixel(chip8_screen *screen, int y, int x);

#endif