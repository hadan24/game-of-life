#include "game-of-life.h"

int main()
{
    Life::gameSetup();

    Life::runGame();

    Life::gameTeardown();
}