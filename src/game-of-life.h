#pragma once

#include "imgui.h"
#include "rlimgui.h"
#include "raylib.h"
#include <vector>

namespace Life {
    constexpr int SCREEN_W = 1280;
    constexpr int SCREEN_H = 720;
    inline int cellSize = 20;

    struct IntVec2 { int x, y; };

    void gameSetup();
    void drawBegin();
    void drawEnd();
    void gameTeardown();

    int cellToPx(int cellCoord);
    int pxToCellNum(int pxCoord);
    int pxToCellVis(int pxCoord);

    class Grid {
    public:
        const int width = SCREEN_W / cellSize;
        const int height = SCREEN_H / cellSize;

        Grid();
        // constructor that takes list of cell coords to start alive

        bool isAlive(int x, int y) const;
        void flipCell(int x, int y);
        void spawnCell(int x, int y);
        void killCell(int x, int y);
        void advanceTicks(int numTicks = 1);

        int neighbors(int x, int y) const;  // for debugging

    private:
        std::vector<char> m_data;
        std::vector<char> m_neighbors;

        enum CellState { ALIVE, DEAD };
        void advTick(); // helper
    };
    void drawGrid(const Grid& grid);
    void handleMouse(Grid& g, const IntVec2& mouseRaw);
}

void Game();