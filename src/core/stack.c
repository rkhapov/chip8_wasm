#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "constants.h"

struct stack* create_stack(int size) {
    if (size <= 0 || size > MAX_STACK_SIZE) {
        return NULL;
    }

    struct stack* stack = malloc(sizeof(struct stack));

    stack->top = -1;
    stack->size = size;
    stack->values = malloc(sizeof(uint16_t) * size);

    return stack;
}

void delete_stack(struct stack *stack) {
    if (stack == NULL) {
        return;
    }

    free(stack->values);
    free(stack);
}

void push(struct stack *stack, uint16_t value) {
    if (is_full(stack)) {
        printf("The stack is full\n");
        abort();
    }

    stack->top++;
    stack->values[stack->top] = value;
}

uint16_t pop(struct stack *stack) {
    if (is_empty(stack)) {
        printf("The stack is empty\n");
        abort();
    }

    uint16_t value = stack->values[stack->top];

    stack->top--;

    return value;
}

uint16_t peek(struct stack *stack) {
    if (is_empty(stack)) {
        printf("The stack is empty\n");
        abort();
    }

    uint16_t value = stack->values[stack->top];

    return value;
}


int is_full(struct stack *stack) {
    return stack->top >= stack->size - 1;
}

int is_empty(struct stack *stack) {
    return stack->top == -1;
}