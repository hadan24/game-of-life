#include "game-of-life.h"

namespace Life {
    void GameSetup() {
        InitWindow(SCREEN_W, SCREEN_H, "test");
        SetTargetFPS(144);
        rlImGuiSetup(true);
    }
    void DrawBegin() {
        BeginDrawing();
        rlImGuiBegin();
        ClearBackground(BLACK);
    }
    void DrawEnd() {
        rlImGuiEnd();
        EndDrawing();
    }
    void GameTeardown() {
        rlImGuiShutdown();
        CloseWindow();
    }

    int CellToPx(int cellCoord) { return cellCoord * cellSize; }
    int PxToCellNum(int pxCoord) { return pxCoord / cellSize; }
    int PxToCellVis(int pxCoord) {
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
    ImGui::Text("mouse x: %d", Life::PxToCellNum(mousedCell.x));
    ImGui::Text("mouse y: %d", Life::PxToCellNum(mousedCell.y));
    ImGui::End();
}

void Game() {
    using Life::SCREEN_H, Life::SCREEN_W, Life::cellSize;
    using Life::CellToPx, Life::PxToCellNum, Life::PxToCellVis;

    while (!WindowShouldClose())
    {
        Life::DrawBegin();

        for (int i = 0; i < SCREEN_W; i++)
            DrawLine(CellToPx(i), 0, CellToPx(i), SCREEN_H, DARKGRAY);
        for (int j = 0; j < SCREEN_H; j++)
            DrawLine(0, CellToPx(j), SCREEN_W, CellToPx(j), DARKGRAY);

        Life::IntVec2 mousedCell = {
            static_cast<int>(GetMouseX()),
            static_cast<int>(GetMouseY())
        };
        DrawRectangle(
            PxToCellVis(mousedCell.x),
            PxToCellVis(mousedCell.y),
            cellSize, cellSize, BLUE
        );

        debugWindow(mousedCell);

        Life::DrawEnd();
    }
}