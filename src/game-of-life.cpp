#include "imgui.h"
#include "rlimgui.h"
#include "raylib.h"

int main()
{
    const int w = 700;
    const int h = 700;
    InitWindow(w, h, "test");
    SetTargetFPS(144);
    rlImGuiSetup(true);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        rlImGuiBegin();
        ClearBackground(DARKGRAY);

        ImGui::Begin("hallo :D", NULL);
        ImGui::Text("hallo from ImGui :D");
        ImGui::End();

        rlImGuiEnd();
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
}
