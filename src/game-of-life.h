#pragma once

#include "imgui.h"
#include "rlimgui.h"
#include "raylib.h"

namespace Life {
    constexpr int SCREEN_W = 1280;
    constexpr int SCREEN_H = 720;
    extern int cellSize;

    struct IntVec2 {
        int x;
        int y;
    };

    void GameSetup();
    void DrawBegin();
    void DrawEnd();
    void GameTeardown();

    int CellToPx(int cellCoord);
    int PxToCellNum(int pxCoord);
    int PxToCellVis(int pxCoord);
}

void App();