#pragma once

#include "imgui.h"
#include "rlimgui.h"
#include "raylib.h"
#include <vector>

namespace Life
{
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
    
    class Grid
    {
    public:
        const int m_width = SCREEN_W / cellSize + 1;
        const int m_height = SCREEN_H / cellSize + 1;

        Grid();
        Grid(std::vector<IntVec2> live);

        bool isAlive(int x, int y) const;
        void flipCell(int x, int y);
        void spawnCell(int x, int y);
        void killCell(int x, int y);
        void advanceTick();
        int  neighbors(int i, int y) const;

    private:
        std::vector<char> m_data;
        std::vector<char> m_neighbors;

        enum CellState { ALIVE, DEAD };
    };

    struct UIData {
        int ticksPerSec = 2;
        IntVec2 mouse = {0, 0};
        bool paused = true;
        bool showDetailedCellState = false;
    };
    void Game();
    void update(Life::Grid& g, Life::UIData& ui, long long& nextTickTime);
    void draw(const Life::Grid& g, const Life::UIData& options);
    void uiWindow(Life::Grid& g, Life::UIData& ui);
}