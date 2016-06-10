#include "App.h"
#include <GL/glew.h>

#define GLEW_STATIC

//--------------------

int main(int argc, char* argv[]) {
    sf::Window window({1280, 720, 32}, "GRAFFAAAAARRRRGH",
                      sf::Style::Default,
                      sf::ContextSettings(8, 2, 16, 4, 0));

    auto a = glewInit();
    if (a != GLEW_OK)
        printf("GLEW init failed %u\n", a);

    App app(argc, argv, window);
    app.loop();

    return 0;
}
