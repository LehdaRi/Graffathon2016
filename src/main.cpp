#include "App.h"
#include <SFML/Audio.hpp>

//--------------------

int main(int argc, char* argv[]) {
    MainWindow mainWindow(1920, 1080, "KOUVOLA.EXE");

    //  LOAD BLOXXXX
    Building::loadBlockMeshes();

    sf::Music music;
	if (!music.openFromFile("res/music.ogg"))
        printf("Error loading res/music.ogg");
    else
        music.play();

    App app(argc, argv, mainWindow);
    app.loop();

    return 0;
}
