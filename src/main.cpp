#include "game-of-life.h"

int main() {
    using Life::SCREEN_W, Life::SCREEN_H;
    Life::GameSetup();

    while (!WindowShouldClose())
    {
        Life::DrawBegin();
        ClearBackground(BLACK);

        const int cellSize = 20;
        for (int i = 0; i < SCREEN_W; i++) {
            DrawLine(i * cellSize, 0, i * cellSize, SCREEN_H, DARKGRAY);
        }
        for (int j = 0; j < SCREEN_H; j++) {
            DrawLine(0, j * cellSize, SCREEN_W, j * cellSize, DARKGRAY);
        }
        DrawRectangle(SCREEN_W / 2, SCREEN_H / 2, cellSize, cellSize, BLUE);

        ImGui::Begin("first gui window", NULL);
        ImGui::Text("hallo from ImGui :D");
        ImGui::End();

        Life::DrawEnd();
    }

    Life::GameTeardown();
}