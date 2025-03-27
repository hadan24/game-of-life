#include "game-of-life.h"
#include <array>

namespace Life {
    void gameSetup() {
        InitWindow(SCREEN_W, SCREEN_H, "test");
        SetTargetFPS(144);
        rlImGuiSetup(true);
    }
    void drawBegin() {
        BeginDrawing();
        rlImGuiBegin();
        ClearBackground(BLACK);
    }
    void drawEnd() {
        rlImGuiEnd();
        EndDrawing();
    }
    void gameTeardown() {
        rlImGuiShutdown();
        CloseWindow();
    }

    void drawGrid(const Grid& grid) {
        for (int i = 0; i < grid.width; i++) {
            for (int j = 0; j < grid.height; j++) {
                if (grid.isAlive(i, j))
                    DrawRectangle(
                        cellToPx(i), cellToPx(j),
                        cellSize, cellSize, BLUE
                    );
            }
        }

        for (int i = 0; i < SCREEN_W; i++)
            DrawLine(cellToPx(i), 0, cellToPx(i), SCREEN_H, DARKGRAY);
        for (int j = 0; j < SCREEN_H; j++)
            DrawLine(0, cellToPx(j), SCREEN_W, cellToPx(j), DARKGRAY);
    }

    int cellToPx(int cellCoord) { return cellCoord * cellSize; }
    int pxToCellNum(int pxCoord) { return pxCoord / cellSize; }
    int pxToCellVis(int pxCoord) {
        return static_cast<int>(pxCoord / cellSize) * cellSize;
    }
}

static void debugWindow(const Life::IntVec2& mousedCell, int ticks) {
    using namespace ImGui;
    Begin("first gui window", NULL);
    Text("hallo from ImGui :D");
    NewLine();
    Text("grid width: %d", Life::SCREEN_W / Life::cellSize);
    Text("grid height: %d", Life::SCREEN_H / Life::cellSize);
    NewLine();
    Text("mouse x: %d", Life::pxToCellNum(mousedCell.x));
    Text("mouse y: %d", Life::pxToCellNum(mousedCell.y));
    NewLine();
    Text("ticks passed: %d", ticks);
    End();
}
static void debugCell(const Life::Grid& g, const Life::IntVec2& mousedCell) {
    using namespace ImGui;
    using Life::pxToCellNum;

    int cellNum = pxToCellNum(mousedCell.x) + (pxToCellNum(mousedCell.y) * g.width);
    std::array<Life::IntVec2, 8> dirs (
        { {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1} }
    );

    Begin("cell examination", NULL);

    bool atYEdge = false, atXEdge = false;
    int aliveNeighbors = 0;

    Text("cell num: %d", cellNum);
    NewLine();

    for (auto& d : dirs) {
        bool tempY = (cellNum + (d.y*g.width) < 0)
            || (cellNum + (d.y*g.width) >= g.width*g.height);
        atYEdge = atYEdge || tempY;

        bool tempX = (cellNum%g.width == 0 && d.x == -1)
            || (cellNum%g.width == g.width-1 && d.x == 1);
        atXEdge = atXEdge || tempX;

        if (tempY || tempX)
            continue;
        else if ( g.isAlive(pxToCellNum(mousedCell.x)+d.x, pxToCellNum(mousedCell.y)+d.y) )
            aliveNeighbors++;
    }

    if (atYEdge)
        Text("at Y edge?: true");
    else Text("at Y edge?: false");
    if (atXEdge)
        Text("at X edge?: true");
    else Text("at X edge?: false");

    Text("neighbors: %d", aliveNeighbors);

    End();
}


void Game() {
    Life::Grid g;
    Life::IntVec2 center = {g.width / 2, g.height / 2};
    g.spawnCell(center.x, center.y);
    g.spawnCell(center.x, center.y-1);
    g.spawnCell(center.x-1, center.y);
    g.spawnCell(center.x, center.y+1);
    g.spawnCell(center.x+1, center.y+1);

    int ticks = 0;

    while (!WindowShouldClose())
    {
        Life::drawBegin();

        Life::drawGrid(g);

        if (IsKeyPressed(KEY_ENTER)) {
            ticks++;
            g.advanceTicks();
        }
        if (IsKeyDown(KEY_SPACE)) {
            ticks++;
            g.advanceTicks();
        }

        Life::IntVec2 mousedCell = {
            static_cast<int>(GetMouseX()),
            static_cast<int>(GetMouseY())
        };
        DrawRectangle(
            Life::pxToCellVis(mousedCell.x),
            Life::pxToCellVis(mousedCell.y),
            Life::cellSize, Life::cellSize, BLUE
        );
        debugWindow(mousedCell, ticks);

        Life::drawEnd();
    }
}