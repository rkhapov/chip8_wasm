#include <stdio.h>
#include "gui/sdl_runner.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
EMSCRIPTEN_KEEPALIVE
#endif
int main(int argc, char **argv) {

    #ifndef __EMSCRIPTEN__
    if (argc < 2) {
        printf("Expected path to CHIP-8 game file\n");
        return 1;
    }

    run_chip8(argv[1]);
    #endif    

    return 0;
}