#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

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

chip8* chip8_instance;
SDL_Window *window;
SDL_Renderer *renderer;

void one_iter();

void run_chip8(const char *path) {
    srand(time(0));
    printf("sdl_runner::run\n");

    chip8_instance = create_chip8();
    load_program_file(chip8_instance, path, STANDARD_PROGRAM_OFFSET);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(one_iter, 500, 1);
    #else
    while (1) {
        one_iter();

        SDL_Delay(2);   
    }
    #endif
}

void one_iter() {
    execute_next(chip8_instance);
    tick_timers(chip8_instance);

    process_events(chip8_instance); 
    draw_screen(renderer, chip8_instance);    
}