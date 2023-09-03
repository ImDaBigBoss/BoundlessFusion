#pragma once

#include <stdint.h>

extern uint32_t screen_width; //Extern
extern uint32_t screen_height; //Extern
extern uint32_t buffer_size;
extern uint32_t* screen_buffer;

void extern_screen_init(); //Extern
void screen_init();

void allocate_buffer(); //Extern

void screen_frame(); //Extern

void extern_screen_destroy(); //Extern
void screen_destroy();
