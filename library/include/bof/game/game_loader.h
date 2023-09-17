#pragma once

#include <bof/game/game_vm.h>

#include <stddef.h>

typedef struct {
    void* data;
    uint64_t size;
} game_file_t;

extern game_vm_t virtual_machine;

game_file_t load_game_file();

void load_game();
void game_loop();
void destroy_game();
