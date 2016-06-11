#include "Window.h"

#include <iostream>

//--------------------

static void error_callback(int error, const char* description) {
        fputs(description, stderr);
        getchar();
}

//--------------------

MainWindow::MainWindow(int width, int height, const char* title)
:       window_p_(nullptr)
{
        glfwSetErrorCallback(&error_callback);
        if (!glfwInit())
                throw std::runtime_error("Failed to initialize GLFW.");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        window_p_ = glfwCreateWindow(width, height, title, NULL, NULL);
        if (!window_p_) {
                glfwTerminate();
                throw std::runtime_error("Failed to open the main window.");
        }

        const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwSetWindowPos(window_p_, (mode->width - width) / 2, (mode->height - height) / 2);

        glfwMakeContextCurrent(window_p_);
        gl::import();
}

MainWindow::~MainWindow(void) {
        glfwDestroyWindow(window_p_);
        glfwTerminate();
}
