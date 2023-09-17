#include <bof/io/debug.h>

#include <stdarg.h>
#include <stdio.h>

//TODO: Understand why only every other tag to replace is replaced.
// Example:
// Input: "%x, %x, %x, %x", values {1, 2, 3, 4}
// Output: "1, 0, 2, 0"
// Expected: "1, 2, 3, 4"

void debug_raw(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void debug_info(const char* format, ...) {
    printf("[INFO] ");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}

void debug_warn(const char* format, ...) {
    printf("[WARN] ");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}

void debug_error(const char* format, ...) {
    printf("[ERROR] ");

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    printf("\n");
}
