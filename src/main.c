#include <stdio.h>
#include "gui/sdl_runner.h"

int main(int argc, char **argv) {

#ifndef __EMSCRIPTEN__
    if (argc < 2) {
        printf("Invalid arguments. Expected CHIP-8 game file\n");
        return 1;
    }

    run(argv[1]);
#endif
    
    return 0;
}