#pragma once

#include "imgui.h"
#include "rlimgui.h"
#include "raylib.h"

namespace Life {
    constexpr int SCREEN_W = 1280;
    constexpr int SCREEN_H = 720;

    void GameSetup();
    void DrawBegin();
    void DrawEnd();
    void GameTeardown();
}