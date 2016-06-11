#include "App.h"
#include "gl_api.hpp"

#define GLEW_STATIC

//--------------------

int main(int argc, char* argv[]) {
    App app(argc, argv);
    app.loop();

    return 0;
}
