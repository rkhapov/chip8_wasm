#include <stdio.h>
#include <emscripten.h>

#include "core/memory.h"
#include "core/constants.h"

EMSCRIPTEN_KEEPALIVE
void test_stack() {
    printf("hello from test!\n");
}

int main(int argc, char **argv) {
    return 0;
}