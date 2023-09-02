#include <io/screen.h>
#include <io/debug.h>

#include <lib_config.h>
#include <lib_main.h>

#include <stdlib.h>

#include <GLFW/glfw3.h>

uint32_t screen_width;
uint32_t screen_height;
extern uint32_t buffer_size;
extern uint32_t* screen_buffer;

GLFWwindow* window;

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
    glfwGetFramebufferSize(window, (int*) &screen_width, (int*) &screen_height);
    glViewport(0, 0, screen_width, screen_height);

    glRasterPos2f(-1, 1);
    glPixelZoom(1, -1);

    window_resized();
}

// --- Exposed functions ---

void extern_screen_init() {
    //Setup GLFW
    glfwSetErrorCallback(error_handle_code);
    if (!glfwInit()) {
        debug_error("Failed to initialize GLFW!");
        lib_exit(EXIT_FAILURE);
    }

    //Create window
    window = glfwCreateWindow(640, 480, LIB_NAME, NULL, NULL);
    if (!window) {
        debug_error("Failed to create GLFW window!");
        error_handle();
        lib_exit(EXIT_FAILURE);
    }

    glfwGetFramebufferSize(window, (int*) &screen_width, (int*) &screen_height);
    glfwSetWindowSizeLimits(window, 300, 200, GLFW_DONT_CARE, GLFW_DONT_CARE);

    //Make window visible
    glfwDefaultWindowHints();
    glfwMakeContextCurrent(window);
    glfwShowWindow(window);

    //TODO: Setup content scaling for HiDPI displays

    //Make the buffer start at the top left
    glRasterPos2f(-1, 1);
    glPixelZoom(1, -1);

    //Setup callbacks
    glfwSetWindowSizeCallback(window, window_resized_int);
}

void screen_frame() {
    if (glfwWindowShouldClose(window)) {
        lib_exit(EXIT_SUCCESS);
    }

    glDrawPixels(screen_width, screen_height, GL_RGBA, GL_UNSIGNED_BYTE, screen_buffer);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void screen_destroy() {
    if (window) {
        glfwDestroyWindow(window);
    }

    glfwTerminate();
}
