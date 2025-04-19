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


    static void debugWindow(const Life::Grid& g, const Life::IntVec2& mousedCell) {
        using namespace ImGui;

        int cellX = Life::pxToCellNum(mousedCell.x), cellY = Life::pxToCellNum(mousedCell.y);
        Begin("first gui window", NULL);
        Text("hallo from ImGui :D");
        NewLine();
        Text("grid width: %d", Life::SCREEN_W / Life::cellSize);
        Text("grid height: %d", Life::SCREEN_H / Life::cellSize);
        NewLine();
        Text("mouse x: %d (%d)", cellX, mousedCell.x);
        Text("mouse y: %d (%d)", cellY, mousedCell.y);
        End();
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
        Life::TimingData t = {
            ui.ticksPerSec,
            time_point_cast<milliseconds>(steady_clock::now())
                .time_since_epoch()
                .count()
        };

        while (!WindowShouldClose())
        {
            ui.mouse = {
                static_cast<int>(GetMouseX()),
                static_cast<int>(GetMouseY())
            };
            update(g, ui, t);


            Life::drawBegin();
            draw(g, ui.mouse);
            debugWindow(g, ui.mouse);
            Life::drawEnd();
        }
    }

    void update(Life::Grid& g, const Life::UIData& ui, TimingData& t)
    {
        using Life::pxToCellNum;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
            g.spawnCell( pxToCellNum(ui.mouse.x), pxToCellNum(ui.mouse.y) );
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            g.killCell( pxToCellNum(ui.mouse.x), pxToCellNum(ui.mouse.y) );
        if (IsKeyPressed(KEY_ENTER))
            g.advanceTick();
        if (IsKeyPressed(KEY_SPACE))
            g.togglePause();


        int tickWaitInterval = 1000 / t.ticksPerSec;  // in milliseconds
        constexpr int MAX_FRAMESKIP = 10;
        int loops = 0;

        using namespace std::chrono;
        auto currTime = time_point_cast<milliseconds>(steady_clock::now())
            .time_since_epoch()
            .count();
        while (currTime > t.nextTickTime && loops < MAX_FRAMESKIP)
        {
            if (!g.paused())
                g.advanceTick();
            t.nextTickTime += tickWaitInterval;
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
}