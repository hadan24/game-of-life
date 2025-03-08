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
    }

    void DrawEnd() {
        rlImGuiEnd();
        EndDrawing();
    }

    void GameTeardown() {
        rlImGuiShutdown();
        CloseWindow();
    }
}