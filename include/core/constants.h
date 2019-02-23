#ifndef __CHIP8_CONSTANTS_H__
#define __CHIP8_CONSTANTS_H__

#define MAX_STACK_SIZE           0x1000
#define RECOMMENDED_STACK_SIZE   0x1000

#define MIN_MEMORY_SIZE          0x400
#define MAX_MEMORY_SIZE          0xFFFF
#define RECOMMENDED_MEMORY_SIZE  0x1000

#define V_REGISTERS_AMOUNT       16
#define V_FLAG_REGISTER          15

#define SCREEN_HEIGHT            32
#define SCREEN_WIDTH             64

#define KEYBOARD_KEYS_COUNT      16

#define PROGRAM_STANDARD_OFFSET  0x200

#endif