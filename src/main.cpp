// #include "GameState.h"
#include "Game.h"

using namespace std;

int main() {
    // GameState game;
    // game.runGame();

    Game::getInstance()->run();
    return 0;
}
