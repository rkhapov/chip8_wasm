#include <stdio.h>
#include <emscripten.h>

#include "core/chip8.h"

EMSCRIPTEN_KEEPALIVE
void test_stack() {
    chip8 *c8 = create_chip8();

    do_key_down(c8->keyboard, 5);

    printf("%d\n", is_key_pressed(c8->keyboard, 5));
    printf("%d\n", is_key_pressed(c8->keyboard, 7));
}

int main(int argc, char **argv) {
    return 0;
}