#include <bof/game/game_loader.h>

#include <bof/io/debug.h>

#include <bof/config.h>
#include <bof/main.h>

#include <stdlib.h>
#include <stdio.h>

game_file_t load_game_file() {
    FILE* file = fopen(LIB_GAME_PATH, "rb");
    if (file == NULL) {
        debug_error("Failed to open game file at ./%s (does it exist?)", LIB_GAME_PATH);
        lib_exit(1);
    }

    fseek(file, 0, SEEK_END);
    uint64_t size = ftell(file);
    rewind(file);

    void* data = malloc(size);
    if (data == NULL) {
        debug_error("Failed to allocate memory for game file");
        lib_exit(1);
    }

    if (fread(data, 1, size, file) != size) {
        debug_error("Failed to read game file");
        lib_exit(1);
    }

    fclose(file);

    return (game_file_t) {
        .data = data,
        .size = size
    };
}
