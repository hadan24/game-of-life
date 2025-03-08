#include "game-of-life.h"

int main()
{
    Life::GameSetup();

    App();

    Life::GameTeardown();
}