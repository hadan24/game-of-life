#pragma once
#include "game-of-life.h"
#include <vector>

namespace Life {
    class Grid {
    public:
        Grid();
        // constructor that takes list of cell coords to start alive

        // should class ensure valid indices?
        bool getCell(int x, int y);
        void flipCell(int x, int y);
        void spawnCell(int x, int y);
        void killCell(int x, int y);
        void advanceTicks(int toAdv = 1);

    private:
        const int m = SCREEN_W / cellSize;
        const int n = SCREEN_H / cellSize;
        std::vector<char> m_data;
    };
}