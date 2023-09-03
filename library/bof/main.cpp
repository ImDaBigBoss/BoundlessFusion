#include <bof/main.h>

#include <bof/io/debug.h>
#include <bof/io/screen.h>

#include <bof/util/rainbow.h>
#include <bof/util/fps.h>

#include <stdlib.h>
#include <signal.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

void lib_init(int argc, char** argv) {
    debug_info("Boundless Fusion v1.0.0");

    //Component initialization
    screen_init();

    //Signal handling
    signal(SIGINT, lib_exit);
    signal(SIGTERM, lib_exit);
}

uint32_t colour_1 = 0;
uint32_t colour_2 = 0;
void lib_loop() {
    frame_tick();
    debug_info("Frame took %f ms (%d FPS)", frame_time, fps);

    colour_2 += 1;
    colour_1 = colour_2;

    uint32_t* ptr = screen_buffer;
    for (int y = 0; y < screen_height; y++) {
        uint32_t line_col = rainbow(colour_1++);

        for (int x = 0; x < screen_width; x++) {
            *ptr = line_col;
            ptr++;
        }
    }

    screen_frame();
}

void lib_exit(int code) {
    debug_raw("\033[0m\n"); //Reset color and newline
    debug_raw("Shutting down...");
    screen_destroy();

    exit(code);
}
