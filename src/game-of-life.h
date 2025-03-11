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

    class Grid {
    public:
        using size_type = std::vector<char>::size_type;
        const size_type width = SCREEN_W / cellSize;
        const size_type height = SCREEN_H / cellSize;

        Grid();
        // constructor that takes list of cell coords to start alive

        bool getCell(size_type x, size_type y) const;
        void flipCell(size_type x, size_type y);
        void spawnCell(size_type x, size_type y);
        void killCell(size_type x, size_type y);
        void advanceTicks(int toAdv = 1);

    private:
        std::vector<char> m_data;

        void advTick(); // helper
    };
    void drawGrid(const Grid& grid);

    int cellToPx(int cellCoord);
    int pxToCellNum(int pxCoord);
    int pxToCellVis(int pxCoord);
}

void Game();