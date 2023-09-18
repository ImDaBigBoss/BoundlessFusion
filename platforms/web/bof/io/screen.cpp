#include <bof/io/screen.h>
#include <bof/io/debug.h>

#include <bof/main.h>

#include <stdlib.h>

#include <emscripten.h>
#include <SDL/SDL.h>

extern screen_buffer_t screen_buffer;

SDL_Surface* screen;

// --- Utils ---

EM_JS(int, get_canvas_width, (), { return canvas.width; });
EM_JS(int, get_canvas_height, (), { return canvas.height; });

void allocate_buffer() {
    screen = SDL_SetVideoMode(get_canvas_width(), get_canvas_height(), 32, SDL_SWSURFACE);

    screen_buffer.width = screen->w;
    screen_buffer.height = screen->h;

    screen_buffer.data = (uint32_t*) screen->pixels;
}

void check_resize() {
    int width = get_canvas_width();
    int height = get_canvas_height();

    if (screen_buffer.width != width || screen_buffer.height != height) {
        screen_buffer.width = width;
        screen_buffer.height = height;
        allocate_buffer();
    }
}

// --- Exposed functions ---

void extern_screen_init() {
    SDL_Init(SDL_INIT_VIDEO);

    //Using EM_ASM, set display_max_width to SCREEN_MAX_WIDTH
    EM_ASM({
        display_max_width = $0;
        display_max_height = $1;
        size_canwas();
    }, SCREEN_MAX_WIDTH, SCREEN_MAX_HEIGHT);

    allocate_buffer();
}

void screen_frame() {
    //Draw screen
    if (SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }
    SDL_Flip(screen);
    check_resize();
    if (SDL_MUSTLOCK(screen)) {
        SDL_LockSurface(screen);
    }

    //Catch events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                lib_exit(1);
                break;
            default:
                break;
        }
    }
}

void screen_destroy() {
    SDL_Quit();
}
