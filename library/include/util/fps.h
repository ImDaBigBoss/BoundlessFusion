#pragma once

#include <time.h>

extern clock_t last_frame;
extern int fps;
extern double frame_time;

void frame_tick();
