#include <bof/main.h>

#include <bof/game/game_loader.h>

#include <bof/io/debug.h>
#include <bof/io/screen.h>

#include <bof/util/fps.h>

#include <string.h>
#include <stdlib.h>
#include <signal.h>

void lib_init(int argc, char** argv) {
    debug_info("Boundless Fusion v1.0.0\n");
    frame_tick(); //Used in the loop function

    //Component initialization
    load_game();
    screen_init();

    game_init();

    //Signal handling
    signal(SIGINT, lib_exit);
    signal(SIGTERM, lib_exit);

    debug_raw("\n");
}

int32_t tmp1 = 0;
bool dir = false;
void lib_loop() {
    frame_tick();
    debug_info("Frame took %f ms (%d FPS)", frame_time, fps);
    //vm_register_dump(&virtual_machine);

    if (dir) {
        tmp1++;
    } else {
        tmp1--;
    }
    if (tmp1 == 20) {
        dir = false;
    } else if (tmp1 == -20) {
        dir = true;
    }

    memset(screen_buffer.data, 0xFF, screen_buffer.size);
    //Draw square with x = 50 + tmp1, y = 50 + tmp1, width = 100, height = 100
    for (int i = 0; i < 100; i++) {
        screen_buffer.data[(50 + tmp1) + (50 + tmp1 + i) * screen_buffer.width] = 0;
        screen_buffer.data[(50 + tmp1 + i) + (50 + tmp1) * screen_buffer.width] = 0;
        screen_buffer.data[(50 + tmp1 + i) + (50 + tmp1 + 100) * screen_buffer.width] = 0;
        screen_buffer.data[(50 + tmp1 + 100) + (50 + tmp1 + i) * screen_buffer.width] = 0;
    }


    game_loop();
    screen_frame();
}

void lib_exit(int code) {
    debug_raw("\n"); //Reset color and newline
    debug_raw("Shutting down with code %d...\n", code);

    screen_destroy();
    destroy_game();

    exit(code);
}
