#include <bof/main.h>

#include <bof/game/game_loader.h>

#include <bof/io/debug.h>
#include <bof/io/screen.h>

#include <bof/util/fps.h>

#include <string.h>
#include <stdlib.h>
#include <signal.h>

void lib_init(int argc, char** argv) {
    debug_info("Boundless Fusion v1.0.0");
    frame_tick(); //Used in the loop function

    //Component initialization
    screen_init();

    //Finally, load the game
    load_game();

    //Signal handling
    signal(SIGINT, lib_exit);
    signal(SIGTERM, lib_exit);

    debug_raw("\n");
}

void lib_loop() {
    frame_tick();
    debug_info("Frame took %f ms (%d FPS)", frame_time, fps);
    //vm_register_dump(&virtual_machine);

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
