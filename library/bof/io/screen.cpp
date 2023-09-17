#include <bof/io/screen.h>
#include <bof/io/debug.h>

#include <bof/game/game_loader.h>

#include <string.h>

screen_buffer_t screen_buffer;

void screen_init() {
    memset(&screen_buffer, 0, sizeof(screen_buffer_t));
    virtual_machine.memory_pool.framebuffer_data = &screen_buffer;

    extern_screen_init();

    debug_info("Screen initialized with size %dx%d", screen_buffer.width, screen_buffer.height);
}
