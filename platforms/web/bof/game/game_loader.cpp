#include <bof/game/game_loader.h>

#include <bof/io/debug.h>

#include <bof/config.h>
#include <bof/main.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <emscripten/fetch.h>

bool game_loaded;
uint64_t game_size;
void* game_data;

EM_JS(void, browser_alert, (char* message), { alert(message); });

void downloadSucceeded(emscripten_fetch_t *fetch) {
    game_data = malloc(fetch->numBytes);
    memcpy(game_data, fetch->data, fetch->numBytes);
    game_size = fetch->numBytes;

    emscripten_fetch_close(fetch);

    game_loaded = true;
}

void downloadFailed(emscripten_fetch_t *fetch) {
    debug_error("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
    EM_ASM(
        alert("Failed to download game binary at game.bof. Please check the console for more information.");
    );

    emscripten_fetch_close(fetch);

    game_size = 0;
    game_data = 0;
    game_loaded = true;
}

game_file_t load_game_file() {
    game_loaded = false;

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = downloadSucceeded;
    attr.onerror = downloadFailed;

    emscripten_fetch(&attr, LIB_GAME_PATH);

    while (!game_loaded) {
        emscripten_sleep(100);
    }
    if (game_data == 0) { //We don't need to print an error message, downloadFailed() already did that
        lib_exit(1);
    }

    return (game_file_t) {
        .data = game_data,
        .size = game_size
    };
}
