#ifndef __EMSCRIPTEN__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>

#include "core/chip8.h"
#include "gui/sdl_runner.h"

byte keys[] = {
    SDLK_x, SDLK_1, SDLK_2, SDLK_3,
    SDLK_q, SDLK_w, SDLK_e, SDLK_a,
    SDLK_s, SDLK_d, SDLK_z, SDLK_c,
    SDLK_4, SDLK_r, SDLK_f, SDLK_v
};

#define WINDOW_HEIGHT  512
#define WINDOW_WIDTH   1024

void process_events(chip8 *chip8) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) exit(0);
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE)
                exit(0);
            for (int i = 0; i < 16; ++i) {
                if (e.key.keysym.sym == keys[i]) {
                    do_key_down(chip8->keyboard, i);
                }
            }
        }
        if (e.type == SDL_KEYUP) {
            for (int i = 0; i < 16; ++i) {
                if (e.key.keysym.sym == keys[i]) {
                    do_key_up(chip8->keyboard, i);
                }
            }
        }
    }
}

void draw_screen(SDL_Renderer *renderer, chip8 *chip8) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect rect;
    rect.h = 16;
    rect.w = 16;

    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            if (get_pixel(chip8->screen, y, x)) {
                rect.x = x * 16;
                rect.y = y * 16;

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void run(const char *path) {
    srand(time(0));

    chip8 *chip8 = create_chip8(path);
    load_program_file(chip8, path, STANDARD_PROGRAM_OFFSET);

    SDL_Window *window = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("Cant init SDL\n");
        abort();
    }

    window = SDL_CreateWindow(
        "CHIP-8", SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, 
        WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("Cant create window!\n");
        abort();
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    while (1) {
        execute_next(chip8);
        tick_timers(chip8);

        process_events(chip8); 
        draw_screen(renderer, chip8);    

        SDL_Delay(2);   
    }
}

#endif //__EMSCRIPTEN__