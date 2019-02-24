#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "api.h"

chip8 *instance = NULL;

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void chip8_start() {
    srand(time(0));
    chip8_end(instance);
    instance = create_chip8();
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void chip8_end(int key) {
    if (instance != NULL) {
        delete_chip8(instance);
    }

    instance = NULL; 
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void chip8_reboot() {
    if (instance != NULL) {
        reset(instance);
    }
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void chip8_load(char *name) {
    if (instance == NULL) {
        return;
    }
    
    load_program_file(instance, name, STANDARD_PROGRAM_OFFSET);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void chip8_do_key_down(int key) {
    if (instance == NULL) {
        return;
    }

    do_key_down(instance->keyboard, key);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void chip8_do_key_up(int key) {
    if (instance == NULL) {
        return;
    }

    do_key_up(instance->keyboard, key);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void chip8_do_tick(int key) {
    if (instance == NULL) {
        return;
    }

    if (instance->exit_code != -1) {
        return;
    }

    execute_next(instance);
    tick_timers(instance);
}


#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
chip8 *get_chip8() {
    return instance;
}


#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int chip8_get_screen_height() {
    return SCREEN_HEIGHT;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int chip8_get_screen_width() {
    return SCREEN_WIDTH;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int chip8_get_pixel(int y, int x) {
    if (instance == NULL) {
        printf("get_pixel::instance are not initialized\n");
        return -1;
    }

    return get_pixel(instance->screen, y, x);
}