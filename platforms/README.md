# Platforms

This directory contains the bootstrap code for specific platforms. Currently supported platforms are:
 * Desktop (Only tested on MacOS)
 * Web

## Adding a new platform

Adding a new platform should be fairly simple. All of this code should work, as the only requried feature is a framebuffer to display on. You can use the existing platforms as a reference for how to implement a new one. The bottom line is that there are some functions that need implementing. You will also need a `Makefile`.
 * Main function (entry point)
 * Screen IO functions (`library/include/bof/io/screen.h` functions marked as external with a comment)
 * Optional debug output functions (`library/bof/include/io/debug.h` functions marked as external with a comment)

The main function should call `lib_init()` and then `lib_loop()` as fast as possible (ideally in a simple while true).
