#include "game-of-life.h"

int main()
{
    Life::IntVec2 center = {
        (Life::SCREEN_W / Life::cellSize) / 2,
        (Life::SCREEN_H / Life::cellSize) / 2
    };

    Life::Game game({
        {center.x,      center.y},
        {center.x,      center.y - 1},
        {center.x - 1,  center.y},
        {center.x,      center.y + 1},
        {center.x + 1,  center.y + 1}
    });
    game.run();
}