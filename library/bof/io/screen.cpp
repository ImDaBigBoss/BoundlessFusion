#include <bof/io/screen.h>
#include <bof/io/debug.h>

extern uint32_t screen_width;
extern uint32_t screen_height;
uint32_t buffer_size;
uint32_t* screen_buffer;

void screen_init() {
    screen_buffer = 0;
    extern_screen_init();
    allocate_buffer();

    debug_info("Screen initialized with size %dx%d", screen_width, screen_height);
}
