#include <stdio.h>
#include <stdlib.h>

#include "screen.h"
#include "constants.h"

chip8_screen *create_screen() {
    chip8_screen *screen = malloc(sizeof(chip8_screen));

    screen->pixels = malloc(sizeof(chip8_pixel_t*) * SCREEN_HEIGHT);

    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        screen->pixels[i] = malloc(sizeof(chip8_pixel_t) * SCREEN_WIDTH);
    }

    clear_screen(screen);

    return screen;
}

void delete_screen(chip8_screen *screen) {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        free(screen->pixels[i]);
    }
    
    free(screen->pixels);
    free(screen);
}

void clear_screen(chip8_screen *screen) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0;  x < SCREEN_WIDTH; x++) {
            screen->pixels[y][x] = 0;
        }
    }
}

chip8_pixel_t set_pixel(chip8_screen *screen, int y, int x, chip8_pixel_t value) {
    if (value != 0 && value != 1) {
        printf("Pixel value cant be %d\n", value);
        abort();
    }

    y = y % SCREEN_HEIGHT;
    x = x % SCREEN_WIDTH;

    screen->pixels[y][x] ^= value;

    // return 1 if collision detected
    if (value == 1 && screen->pixels[y][x] == 0)
        return 1;

    return 0;
}

chip8_pixel_t get_pixel(chip8_screen *screen, int y, int x) {
    return screen->pixels[y][x];
}