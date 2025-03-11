#include "game-of-life.h"

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
                if (grid.getCell(i, j))
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

static void debugWindow(const Life::IntVec2& mousedCell) {
    ImGui::Begin("first gui window", NULL);
    ImGui::Text("hallo from ImGui :D");
    ImGui::NewLine();
    ImGui::Text("grid width: %d", Life::SCREEN_W / Life::cellSize);
    ImGui::Text("grid height: %d", Life::SCREEN_H / Life::cellSize);
    ImGui::NewLine();
    ImGui::Text("mouse x: %d", Life::pxToCellNum(mousedCell.x));
    ImGui::Text("mouse y: %d", Life::pxToCellNum(mousedCell.y));
    ImGui::End();
}

void Game() {
    using Life::SCREEN_H, Life::SCREEN_W, Life::cellSize;
    using Life::cellToPx, Life::pxToCellVis;

    Life::Grid g;
    g.spawnCell(10, 10);
    g.spawnCell(11, 10);
    g.spawnCell(11, 11);
    g.spawnCell(10, 11);

    while (!WindowShouldClose())
    {
        Life::drawBegin();

        Life::drawGrid(g);

        Life::IntVec2 mousedCell = {
            static_cast<int>(GetMouseX()),
            static_cast<int>(GetMouseY())
        };
        DrawRectangle(
            pxToCellVis(mousedCell.x),
            pxToCellVis(mousedCell.y),
            cellSize, cellSize, BLUE
        );

        debugWindow(mousedCell);

        Life::drawEnd();
    }
}