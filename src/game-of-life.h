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
    struct UIData {
        int ticksPerSec = 2;
        IntVec2 mouse;
        bool paused = true;
        bool showDetailedCellState = false;
    };
    struct TimingData {
        int ticksPerSec;
        long long nextTickTime;
    };

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
        const int width = SCREEN_W / cellSize;
        const int height = SCREEN_H / cellSize;

        Grid();
        Grid(std::vector<IntVec2> live);

        bool isAlive(int x, int y) const;
        void flipCell(int x, int y);
        void spawnCell(int x, int y);
        void killCell(int x, int y);
        void advanceTick();

        void togglePause();
        bool paused() const;

    private:
        std::vector<char> m_data;
        std::vector<char> m_neighbors;
        bool m_paused = true;

        enum CellState { ALIVE, DEAD };
    };
    void drawGrid(const Grid& grid);

    void Game();
    void update(Life::Grid& g, const Life::IntVec2& mouse, TimingData& t);
    void draw(const Life::Grid& g, const Life::IntVec2& mouse);
}