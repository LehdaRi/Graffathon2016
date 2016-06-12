#include "App.h"

//--------------------

int main(int argc, char* argv[]) {
    MainWindow mainWindow(1024, 768, "KOUVOLA.EXE");

    //  LOAD BLOXXXX
    Building::loadBlockMeshes();

    App app(argc, argv, mainWindow);
    app.loop();

    return 0;
}
