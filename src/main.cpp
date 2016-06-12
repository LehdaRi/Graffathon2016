#include "App.h"

//--------------------

int main(int argc, char* argv[]) {
    MainWindow mainWindow(1440, 900, "KOUVOLA.EXE");

    //  LOAD BLOXXXX
    Building::loadBlockMeshes();

    App app(argc, argv, mainWindow);
    app.loop();

    return 0;
}
