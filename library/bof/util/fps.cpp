#include <bof/util/fps.h>

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

clock_t last_frame;
int fps;
double frame_time;

void frame_tick() {
    clock_t this_frame = clock();

    frame_time = ((double) (this_frame - last_frame)) / CLOCKS_PER_MS;
    fps = 1000 / frame_time;

    last_frame = this_frame;
}