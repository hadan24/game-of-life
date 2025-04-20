#include "game-of-life.h"
#include <chrono>
#include <array>

namespace Life
{
    void gameSetup()
    {
        InitWindow(SCREEN_W, SCREEN_H, "test");
        SetTargetFPS(144);
        rlImGuiSetup(true);
    }
    void drawBegin()
    {
        BeginDrawing();
        rlImGuiBegin();
        ClearBackground(BLACK);
    }
    void drawEnd()
    {
        rlImGuiEnd();
        EndDrawing();
    }
    void gameTeardown()
    {
        rlImGuiShutdown();
        CloseWindow();
    }

    int cellToPx(int cellCoord) { return cellCoord * cellSize; }
    int pxToCellNum(int pxCoord) { return pxCoord / cellSize; }
    int pxToCellVis(int pxCoord) {
        return static_cast<int>(pxCoord / cellSize) * cellSize;
    }


    void Game()
    {
        Life::IntVec2 center = {
            (Life::SCREEN_W / Life::cellSize) / 2,
            (Life::SCREEN_H / Life::cellSize) / 2 
        };
        Life::Grid g({
            {center.x,      center.y},
            {center.x,      center.y - 1},
            {center.x - 1,  center.y},
            {center.x,      center.y + 1},
            {center.x + 1,  center.y + 1}
        });

        using namespace std::chrono;
        Life::UIData ui;
        long long nextTickTime = time_point_cast<milliseconds>(steady_clock::now())
            .time_since_epoch()
            .count();

        while (!WindowShouldClose())
        {
            ui.mouse = {
                static_cast<int>(GetMouseX()),
                static_cast<int>(GetMouseY())
            };
            update(g, ui, nextTickTime);


            Life::drawBegin();
            draw(g, ui.mouse);
            uiWindow(g, ui);
            Life::drawEnd();
        }
    }

    void update(Life::Grid& g, Life::UIData& uiData, long long& nextTickTime)
    {
        using Life::pxToCellNum;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
            g.spawnCell(pxToCellNum(uiData.mouse.x), pxToCellNum(uiData.mouse.y));
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            g.killCell(pxToCellNum(uiData.mouse.x), pxToCellNum(uiData.mouse.y));
        if (
            (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) &&
            IsKeyDown(KEY_ENTER)
        )
            g.advanceTick();
        else if (IsKeyPressed(KEY_ENTER))
            g.advanceTick();
        if (IsKeyPressed(KEY_SPACE))
            uiData.paused = !uiData.paused;


        int tickWaitInterval = 1000 / uiData.ticksPerSec;  // in milliseconds
        constexpr int MAX_FRAMESKIP = 10;
        int loops = 0;

        using namespace std::chrono;
        auto currTime = time_point_cast<milliseconds>(steady_clock::now())
            .time_since_epoch()
            .count();
        while (currTime > nextTickTime && loops < MAX_FRAMESKIP)
        {
            if (!uiData.paused)
                g.advanceTick();
            nextTickTime += tickWaitInterval;
            loops++;
        }
    }
    void draw(const Life::Grid& g, const Life::IntVec2& mouse)
    {
        // Draw live cells
        for (int i = 0; i < g.width; i++)
            for (int j = 0; j < g.height; j++)
                if (g.isAlive(i, j))
                    DrawRectangle(cellToPx(i), cellToPx(j), cellSize, cellSize, BLUE);

        // Draw grid lines
        for (int i = 0; i < SCREEN_W; i++)
            DrawLine(cellToPx(i), 0, cellToPx(i), SCREEN_H, DARKGRAY);
        for (int j = 0; j < SCREEN_H; j++)
            DrawLine(0, cellToPx(j), SCREEN_W, cellToPx(j), DARKGRAY);

        // Highlight moused cell
        DrawRectangle(
            Life::pxToCellVis(mouse.x), Life::pxToCellVis(mouse.y),
            Life::cellSize, Life::cellSize, DARKBLUE
        );
    }
    void uiWindow(Life::Grid& g, Life::UIData& ui) {
        using namespace ImGui;

        int cellX = Life::pxToCellNum(ui.mouse.x);
        int cellY = Life::pxToCellNum(ui.mouse.y);
        Begin("Simulation Controls/Settings", NULL);

        Text("FPS: %d", static_cast<int>(ImGui::GetIO().Framerate));
        NewLine();

        SliderInt("Sim Speed", &(ui.ticksPerSec), 1, 10, "%d");
        if ( Button("Pause/Play") )
            ui.paused = !ui.paused;
        SameLine(); Text(ui.paused ? "Paused" : "Playing");
        if ( Button("Next Tick") )
            g.advanceTick();
        NewLine();

        if ( Button("Show Detailed Cell States"))
            ui.showDetailedCellState = !ui.showDetailedCellState;
        SameLine();
        Text(ui.showDetailedCellState ? "On" : "Off");
        NewLine();

        Text("mouse x: %d (%d)", cellX, ui.mouse.x);
        Text("mouse y: %d (%d)", cellY, ui.mouse.y);
        End();
    }
}