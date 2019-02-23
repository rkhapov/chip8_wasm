#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "core/chip8.h"

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void test_stack() {
    chip8 *chip8 = create_chip8();

    printf("%x\n", get_next_opcode(chip8));
}

int main(int argc, char **argv) {
    printf("at main!\n");
    return 0;
}