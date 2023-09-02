#include <io/screen.h>
#include <io/debug.h>

#include <lib_main.h>

#include <stdlib.h>

#include <emscripten.h>
#include <SDL/SDL.h>

uint32_t screen_width;
uint32_t screen_height;
extern uint32_t buffer_size;
extern uint32_t* screen_buffer;

SDL_Surface* screen;

// --- Utils ---

EM_JS(int, get_canvas_width, (), { return canvas.width; });
EM_JS(int, get_canvas_height, (), { return canvas.height; });

// --- Exposed functions ---

void extern_screen_init() {
    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(get_canvas_width(), get_canvas_height(), 32, SDL_SWSURFACE);

    screen_width = screen->w;
    screen_height = screen->h;
}

void screen_frame() {
    //Draw screen
    if (SDL_MUSTLOCK(screen)) {
        SDL_LockSurface(screen);
    }

    memcpy(screen->pixels, screen_buffer, buffer_size);

    if (SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }

    SDL_Flip(screen);

    //Catch events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                lib_exit(EXIT_SUCCESS);
                break;
            default:
                break;
        }
    }
}

void screen_destroy() {
    SDL_Quit();
}
