#include <bof/game/game_loader.h>
#include <bof/game/game.h>

#include <bof/io/debug.h>

#include <bof/main.h>

#include <stdlib.h>
#include <string.h>

game_vm_t virtual_machine;

void read_game_file_v1(game_program_t* program, game_file_t* game_file) {
    uint8_t* data = (uint8_t*) game_file->data;
    game_file_header_t* header = (game_file_header_t*) data;

    program->size = header->size - sizeof(game_file_header_t);
    program->data = malloc(program->size + 9); //+9 for halting instruction & safety padding
    memset(program->data, 0, program->size + 9);
    memcpy(program->data, data + sizeof(game_file_header_t), program->size);

    program->start_address = header->start_address - sizeof(game_file_header_t); //Subtract header size to get correct address
}

void load_game() {
    debug_info("Loading game...");

    game_file_t game_file = load_game_file();
    debug_info("Game file loaded (%d bytes)", game_file.size);

    //Place to store the extracted program from the game file
    game_program_t program;

    //Read the game file
    game_file_header_t* header = (game_file_header_t*) game_file.data;
    if (header->magic != GAME_MAGIC) {
        debug_error("Invalid game file magic");
        lib_exit(1);
    }
    switch (header->version) {
        case 1:
            read_game_file_v1(&program, &game_file);
            break;
        default:
            debug_error("Unsupported game file version");
            lib_exit(1);
    }

    //Free the game file data
    free(game_file.data);

    debug_info("Starting virual runtime...");
    vm_init(&virtual_machine, &program);

    debug_info("Running game init...");
    vm_run(&virtual_machine, program.start_address);
    debug_info("Game loaded!");
}

void game_loop() {
    vm_run(&virtual_machine, virtual_machine.game_loop_address);
}

void destroy_game() {
    debug_info("Destroying game...");

    vm_destroy(&virtual_machine);
}
