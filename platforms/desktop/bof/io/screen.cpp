#include <bof/io/screen.h>
#include <bof/io/debug.h>

#include <bof/config.h>
#include <bof/main.h>

#include <stdlib.h>
#include <string.h>

#include <GLFW/glfw3.h>

extern screen_buffer_t screen_buffer;

GLFWwindow* window;
uint32_t buffer[SCREEN_MAX_WIDTH * SCREEN_MAX_HEIGHT];

// --- Utils ---

void error_handle_code(int code, const char* description) {
    debug_error("GLFW Error %d: %s", code, description);
}

void error_handle() {
    const char* description;
    int code = glfwGetError(&description);

    if (code != GLFW_NO_ERROR) {
        error_handle_code(code, description);
    }
}

void window_resized_int(GLFWwindow* window, int width, int height) {
    glfwGetFramebufferSize(window, (int*) &screen_buffer.width, (int*) &screen_buffer.height);
    glViewport(0, 0, screen_buffer.width, screen_buffer.height);

    //If the window is too large, scale the buffer down keeping the aspect ratio
    float scale = 1.0f;
    if (screen_buffer.width > SCREEN_MAX_WIDTH || screen_buffer.height > SCREEN_MAX_HEIGHT) {
        float aspect_ratio = (float) screen_buffer.width / (float) screen_buffer.height;
        float old_width = screen_buffer.width;
        if (screen_buffer.width > SCREEN_MAX_WIDTH) {
            screen_buffer.width = SCREEN_MAX_WIDTH;
            screen_buffer.height = (int) (screen_buffer.width / aspect_ratio);
        } else {
            screen_buffer.height = SCREEN_MAX_HEIGHT;
            screen_buffer.width = (int) (screen_buffer.height * aspect_ratio);
        }
        scale = old_width / (float) screen_buffer.width;
    }

    //TODO: Setup content scaling for HiDPI displays

    //Make the buffer start at the top left
    glRasterPos2f(-1, 1);
    glPixelZoom(scale, -scale);
}

// --- Exposed functions ---

void extern_screen_init() {
    //Setup GLFW
    glfwSetErrorCallback(error_handle_code);
    if (!glfwInit()) {
        debug_error("Failed to initialize GLFW!");
        lib_exit(1);
    }

    //Create window
    window = glfwCreateWindow(640, 480, LIB_NAME, NULL, NULL);
    if (!window) {
        debug_error("Failed to create GLFW window!");
        error_handle();
        lib_exit(1);
    }
    
    screen_buffer.data = buffer;

    glfwSetWindowSizeLimits(window, 300, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);

    //Make window visible
    glfwDefaultWindowHints();
    glfwMakeContextCurrent(window);
    glfwShowWindow(window);

    window_resized_int(window, 0, 0);

    //Setup callbacks
    glfwSetWindowSizeCallback(window, window_resized_int);
}

void screen_frame() {
    if (glfwWindowShouldClose(window)) {
        lib_exit(1);
    }

    glDrawPixels(screen_buffer.width, screen_buffer.height, GL_RGBA, GL_UNSIGNED_BYTE, screen_buffer.data);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void screen_destroy() {
    if (window) {
        glfwDestroyWindow(window);
    }

    glfwTerminate();
}
