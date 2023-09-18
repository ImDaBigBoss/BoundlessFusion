#pragma once

#include <stdint.h>

#define SCREEN_MAX_WIDTH 4096
#define SCREEN_MAX_HEIGHT 2160

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t* data;
} screen_buffer_t;

extern screen_buffer_t screen_buffer;

void extern_screen_init(); //Extern
void screen_init();

void screen_frame(); //Extern

void extern_screen_destroy(); //Extern
void screen_destroy();
