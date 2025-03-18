#pragma once
#include <vector>
#include "src/game-of-life.h"

class GridBits {
public:
    const int width = Life::SCREEN_W / Life::cellSize;
    const int height = Life::SCREEN_H / Life::cellSize;

    GridBits();
    // constructor that takes list of cell coords to start alive

    bool checkCell(int x, int y) const;
    void flipCell(int x, int y);
    void spawnCell(int x, int y);
    void killCell(int x, int y);
    void advanceTicks(int numTicks = 1);

    void print() const;
    unsigned char getCell(int x, int y) const;

private:
    std::vector<unsigned char> m_data;  // 0-8 count live neighbors, bit 4 is alive bit

    enum CellState : unsigned char { ALIVE = 16, DEAD = 0 };
    void advTick();   // helper
};