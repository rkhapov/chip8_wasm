#include <stdio.h>
#include <emscripten.h>

#include "core/stack.h"

EMSCRIPTEN_KEEPALIVE
void test_stack() {
    struct stack *stack = create_stack(5);

    for (int i = 0; i < 5; i++) {
        push(stack, i);
    }

    for (int i = 0; i < 5; i++) {
        printf("Popping %u\n", pop(stack));
    }
    
    delete_stack(stack);
}

int main(int argc, char **argv) {
    return 0;
}