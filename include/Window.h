#ifndef WINDOW_H
#define WINDOW_H

#include "gl_api.hpp"

class MainWindow {
public:
        MainWindow(int width, int height, const char* title);
        MainWindow(const MainWindow&) = delete;
        ~MainWindow(void);

        MainWindow& operator=(const MainWindow&) = delete;
        operator GLFWwindow*(void) const {return window_p_;}
private:
        GLFWwindow* window_p_;
};

#endif // WINDOW_H
