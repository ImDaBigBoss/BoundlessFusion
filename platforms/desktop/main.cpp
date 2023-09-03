#include <bof/main.h>

int main(int argc, char** argv) {
    lib_init(argc, argv);

    while (1) {
        lib_loop();
    }

    return 0;
}
