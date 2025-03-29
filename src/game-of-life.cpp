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

    void handleMouse(Grid& g, const IntVec2& mouseRaw) {
        DrawRectangle(
            pxToCellVis(mouseRaw.x), pxToCellVis(mouseRaw.y),
            cellSize, cellSize, DARKBLUE
        );

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
            g.spawnCell( pxToCellNum(mouseRaw.x), pxToCellNum(mouseRaw.y) );
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            g.killCell( pxToCellNum(mouseRaw.x), pxToCellNum(mouseRaw.y) );
    }
}

static void debugWindow(const Life::Grid& g, const Life::IntVec2& mousedCell, int ticks) {
    using namespace ImGui;

    int cellX = Life::pxToCellNum(mousedCell.x), cellY = Life::pxToCellNum(mousedCell.y);
    Begin("first gui window", NULL);
    Text("hallo from ImGui :D");
    NewLine();
    Text("grid width: %d", Life::SCREEN_W / Life::cellSize);
    Text("grid height: %d", Life::SCREEN_H / Life::cellSize);
    NewLine();
    Text("mouse x: %d (%d)", cellX, mousedCell.x);
    Text("mouse y: %d (%d)", cellY, mousedCell.y);
    Text( "num neighbors: %d", g.neighbors(cellX, cellY) );
    NewLine();
    Text("ticks passed: %d", ticks);
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

        if (IsKeyPressed(KEY_ENTER) || IsKeyDown(KEY_SPACE)) {
            ticks++;
            g.advanceTicks();
        }
        Life::IntVec2 mousedCell = {
            static_cast<int>(GetMouseX()),
            static_cast<int>(GetMouseY())
        };
        Life::handleMouse(g, mousedCell);
        debugWindow(g, mousedCell, ticks);

        Life::drawEnd();
    }
}