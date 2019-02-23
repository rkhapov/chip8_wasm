#ifndef __CHIP8_JS_API__H__
#define __CHIP8_JS_API__H__

#include "core/chip8.h"

void chip8_start();
void chip8_end();
void chip8_reboot();
void chip8_load(char *name);
void chip8_do_key_down(int key);
void chip8_do_key_up(int key);
void chip8_do_tick();

chip8 *get_chip8();

int chip8_get_screen_height();
int chip8_get_screen_width();
int chip8_get_pixel(int y, int x);

#endif

