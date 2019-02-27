#include <stdio.h>
#include "gui/sdl_runner.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
EMSCRIPTEN_KEEPALIVE
#endif
int main(int argc, char **argv) {

    printf("Application was started\n");

    run_chip8("games/PONG2");

    return 0;
}