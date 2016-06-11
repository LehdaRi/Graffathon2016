#include "App.h"
#include "gl_api.hpp"

#define GLEW_STATIC

//--------------------

int main(int argc, char* argv[]) {
    MainWindow mainWindow(1024, 768, "GRAFFAAAAAARGH");

    //  LOAD BLOXXXX
    Building::loadBlockMeshes();

    App app(argc, argv, mainWindow);
    app.loop();

    return 0;
}
