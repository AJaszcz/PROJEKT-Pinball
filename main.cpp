#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Game.h"

using namespace sf;
int main() {
    //init Game engine
    Game game;

    //Game Loop
    int i(0);
    while (game.running()) {

        //Update
        game.update();
        //Render
        game.render();
    }
}