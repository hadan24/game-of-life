#pragma once
#include <vector>

namespace Life
{
    constexpr int SCREEN_W = 1280;
    constexpr int SCREEN_H = 720;
    inline int cellSize = 20;

    struct IntVec2 { int x, y; };

    void gameSetup();
    void gameTeardown();
    void drawBegin();
    void drawEnd();

    
    class Grid
    {
    public:
        const int m_width = SCREEN_W / cellSize;
        const int m_height = SCREEN_H / cellSize;

        Grid();
        Grid(std::vector<IntVec2> live);

        bool isAlive(int x, int y) const;
        void flipCell(int x, int y);
        void spawnCell(int x, int y);
        void killCell(int x, int y);
        void advanceTick();
        int  neighbors(int x, int y) const;
        int  cellToPx(int cellCoord) const { return cellCoord * cellSize; }
        int  pxToCellNum(int pxCoord) const { return pxCoord / cellSize; }
        int  pxToCellVis(int pxCoord) const {
            return static_cast<int>(pxCoord / cellSize) * cellSize;
        }
        int clampX(int x) const;
        int clampY(int) const;
        
        void setEdgeWrap(bool val);

    private:
        std::vector<char> m_data;
        std::vector<char> m_neighbors;

        bool edgeWrap = false;

        enum CellState { DEAD, ALIVE };
        bool inBounds(int x, int y) const;
        
        int wrapX(int x) const;
        int wrapY(int y) const;
    };

    struct UIData {
        int ticksPerSec = 2;
        IntVec2 mouse = {0, 0};
        bool paused = true;
        bool showDetailedState = false;
        bool screenWrap = false;
    };
    
    void update(Life::Grid& g, Life::UIData& ui, double& nextTickTime);
    void draw(const Life::Grid& g, const Life::UIData& options);
    void ui(Life::Grid& g, Life::UIData& ui);

    void runGame();
}