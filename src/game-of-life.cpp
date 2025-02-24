#include <iostream>
#include "imgui/imgui.h"
#include "rlimgui/imgui_impl_raylib.h"
#include "raylib.h"

int main()
{
    const int w = 700;
    const int h = 700;
    InitWindow(w, h, "test");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();
}
