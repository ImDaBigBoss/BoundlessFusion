#include <bof/main.h>

#include <bof/game/game_loader.h>

#include <bof/io/debug.h>
#include <bof/io/screen.h>

#include <bof/util/fps.h>

#include <stdlib.h>
#include <signal.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

void lib_init(int argc, char** argv) {
    debug_info("Boundless Fusion v1.0.0");

    //Component initialization
    screen_init();

    //Finally, load the game
    load_game();

    //Signal handling
    signal(SIGINT, lib_exit);
    signal(SIGTERM, lib_exit);
}

void lib_loop() {
    frame_tick();
    debug_info("Frame took %f ms (%d FPS)", frame_time, fps);
    debug_raw("R1 = 0x%x, R2 = 0x%x\nR3 = 0x%x, R4 = 0x%x\nSP = 0x%x, PC = 0x%x\n\n", virtual_machine.registers.r1, virtual_machine.registers.r2, virtual_machine.registers.r3, virtual_machine.registers.r4, virtual_machine.registers.sp, virtual_machine.registers.pc);

    //TODO: Call the game loop

    screen_frame();
}

void lib_exit(int code) {
    debug_raw("\n"); //Reset color and newline
    debug_raw("Shutting down with code %d...\n", code);

    screen_destroy();
    destroy_game();

    exit(code);
}
