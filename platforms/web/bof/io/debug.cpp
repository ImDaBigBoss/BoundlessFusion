#include <bof/io/debug.h>

#include <stdarg.h>
#include <stdio.h>

void debug_raw(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void debug_info(const char* format, ...) {
    printf("[INFO] "); // Green

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}

void debug_warn(const char* format, ...) {
    printf("[WARN] "); // Yellow

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}

void debug_error(const char* format, ...) {
    printf("[ERROR] "); // Red

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}
