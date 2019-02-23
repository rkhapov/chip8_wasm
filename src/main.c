#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "core/chip8.h"

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void test_stack() {
    chip8 *c8 = create_chip8();

    do_key_down(c8->keyboard, 5);

    printf("%d\n", is_key_pressed(c8->keyboard, 5));
    printf("%d\n", is_key_pressed(c8->keyboard, 7));
    printf("llloooool with archive and .o!\n");
}

int main(int argc, char **argv) {
    printf("at main!\n");
    return 0;
}