#ifndef __CHIP8_MEMORY_H__
#define __CHIP8_MEMORY_H__

#include "chip8_types.h"

byte* create_memory(int size);
void delete_memory(byte *memory);

#endif