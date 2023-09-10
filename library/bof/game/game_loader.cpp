#include <bof/game/game_loader.h>
#include <bof/game/game.h>

#include <bof/io/debug.h>

#include <bof/main.h>

#include <stdlib.h>
#include <string.h>

game_vm_t virtual_machine;

void read_game_file_v1(game_program_t* program, game_file_t* game_file) {
    game_file_header_t* header = (game_file_header_t*) game_file->data;

    uint32_t data_size = game_file->size - sizeof(game_file_header_t);
    program->data = malloc(data_size);
    memcpy(program->data, header + sizeof(game_file_header_t), data_size);
    program->size = data_size;

    program->start_address = header->start_address;
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
}

void destroy_game() {
    debug_info("Destroying game...");

    vm_destroy(&virtual_machine);
}
