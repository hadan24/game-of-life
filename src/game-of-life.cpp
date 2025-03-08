#include "game-of-life.h"

namespace Life {
    int cellSize = 20;

    void GameSetup() {
        InitWindow(SCREEN_W, SCREEN_H, "test");
        SetTargetFPS(144);
        rlImGuiSetup(true);
    }
    void DrawBegin() {
        BeginDrawing();
        rlImGuiBegin();
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

void App() {
    using Life::SCREEN_H, Life::SCREEN_W, Life::cellSize;
    using Life::CellToPx, Life::PxToCellNum, Life::PxToCellVis;

    while (!WindowShouldClose())
    {
        Life::DrawBegin();
        ClearBackground(BLACK);

        for (int i = 0; i < SCREEN_W; i++)
            DrawLine(CellToPx(i), 0, CellToPx(i), SCREEN_H, DARKGRAY);
        for (int j = 0; j < SCREEN_H; j++)
            DrawLine(0, CellToPx(j), SCREEN_W, CellToPx(j), DARKGRAY);

        Vector2 mouse = GetMousePosition();
        DrawRectangle(
            PxToCellVis(mouse.x),
            PxToCellVis(mouse.y),
            cellSize, cellSize, BLUE
        );

        ImGui::Begin("first gui window", NULL);
        ImGui::Text("hallo from ImGui :D");
        ImGui::NewLine();
        ImGui::Text("grid width: %d", SCREEN_W / cellSize);
        ImGui::Text("grid height: %d", SCREEN_H / cellSize);
        ImGui::NewLine();
        ImGui::Text("mouse x: %d", PxToCellNum((int)mouse.x));
        ImGui::Text("mouse y: %d", PxToCellNum((int)mouse.y));
        ImGui::End();

        Life::DrawEnd();
    }
}