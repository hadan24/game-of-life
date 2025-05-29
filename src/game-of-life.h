#pragma once
#include <vector>

namespace Life
{
    constexpr int SCREEN_W = 1280;
    constexpr int SCREEN_H = 720;
    inline int cellSize = 20;

    struct IntVec2 { int x, y; };
    struct UIData {
        IntVec2 mouse = { 0, 0 };
        int ticksPerSec = 2;
        bool paused = true;
        bool showDetailedState = false;
        bool screenWrap = false;
    };
    
    class Grid
    {
    public:
        const int m_width = SCREEN_W / cellSize;
        const int m_height = SCREEN_H / cellSize;

        Grid();
        Grid(const std::vector<IntVec2>& live);
        Grid(std::vector<IntVec2>&& live);

        bool isAlive(int x, int y) const;
        void flipCell(int x, int y);
        void spawnCell(int x, int y);
        void killCell(int x, int y);
        void advanceTick();
        int neighbors(int x, int y) const;
        
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

    class Game
    {
    public:
        Game();
        Game(const std::vector<IntVec2>& live);
        Game(std::vector<IntVec2>&& live);
        ~Game();

        void run();

    private:
        Grid g;
        UIData options;
        double nextTickTime;

        void drawBegin() const;
        void drawEnd() const;
        int cellToPx(int cellCoord) const;
        int pxToCellNum(int pxCoord) const;

        void update();
        void draw() const;
        void ui();
    };
}