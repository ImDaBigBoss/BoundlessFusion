#include <lib_main.h>

#include <io/debug.h>

#include <emscripten.h>
#include <emscripten/html5.h>

EM_BOOL one_iter(double time, void* userData) {
    lib_loop();
    return EM_TRUE; //True to keep looping
}

int main(int argc, char** argv) {
    lib_init();

    emscripten_request_animation_frame_loop(one_iter, 0);

    return 0;
}
