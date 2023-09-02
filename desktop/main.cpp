#include <lib_main.h>

int main(int argc, char** argv) {
    lib_init();

    while (1) {
        lib_loop();
    }

    return 0;
}
