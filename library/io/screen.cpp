#include <io/screen.h>
#include <io/debug.h>

#include <stdlib.h>
#include <string.h>

extern uint32_t screen_width;
extern uint32_t screen_height;
uint32_t buffer_size;
uint32_t* screen_buffer;

void screen_init() {
    screen_buffer = 0;
    extern_screen_init();

    buffer_size = screen_width * screen_height * sizeof(uint32_t);

    screen_buffer = (uint32_t*) malloc(buffer_size);
    memset(screen_buffer, 0, buffer_size);

    debug_info("Screen initialized with size %dx%d", screen_width, screen_height);
}

void window_resized() {
    if (screen_buffer) {
        free(screen_buffer);
    }

    buffer_size = screen_width * screen_height * sizeof(uint32_t);

    screen_buffer = (uint32_t*) malloc(buffer_size);
    memset(screen_buffer, 0, buffer_size);

    debug_info("Screen resized to %dx%d", screen_width, screen_height);
}
