#include <stdio.h>
#include <stdlib.h>

#include "core/stack.h"
#include "core/constants.h"

chip8_stack* create_stack(int size) {
    if (size <= 0 || size > MAX_STACK_SIZE) {
        return NULL;
    }

    chip8_stack* stack = malloc(sizeof(stack));

    stack->top = -1;
    stack->size = size;
    stack->values = malloc(sizeof(word) * size);

    return stack;
}

void delete_stack(chip8_stack *stack) {
    if (stack == NULL) {
        return;
    }

    free(stack->values);
    free(stack);
}

void push(chip8_stack *stack, word value) {
    if (is_full(stack)) {
        printf("The stack is full\n");
        abort();
    }

    stack->top++;
    stack->values[stack->top] = value;
}

word pop(chip8_stack *stack) {
    if (is_empty(stack)) {
        printf("The stack is empty\n");
        abort();
    }

    word value = stack->values[stack->top];

    stack->top--;

    return value;
}

word peek(chip8_stack *stack) {
    if (is_empty(stack)) {
        printf("The stack is empty\n");
        abort();
    }

    word value = stack->values[stack->top];

    return value;
}


int is_full(chip8_stack *stack) {
    return stack->top >= stack->size - 1;
}

int is_empty(chip8_stack *stack) {
    return stack->top == -1;
}