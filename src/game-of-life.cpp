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

    void drawGrid(const Grid& grid)
    {
        for (int i = 0; i < grid.width; i++)
            for (int j = 0; j < grid.height; j++)
                if (grid.isAlive(i, j))
                    DrawRectangle(cellToPx(i), cellToPx(j), cellSize, cellSize, BLUE);

        for (int i = 0; i < SCREEN_W; i++)
            DrawLine(cellToPx(i), 0, cellToPx(i), SCREEN_H, DARKGRAY);
        for (int j = 0; j < SCREEN_H; j++)
            DrawLine(0, cellToPx(j), SCREEN_W, cellToPx(j), DARKGRAY);
    }

    int cellToPx(int cellCoord) { return cellCoord * cellSize; }
    int pxToCellNum(int pxCoord) { return pxCoord / cellSize; }
    int pxToCellVis(int pxCoord) {
        return static_cast<int>(pxCoord / cellSize) * cellSize;
    }
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
    using namespace std::chrono;

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

    int ticks_per_sec = 2;
    int tick_wait_interval = 1000 / ticks_per_sec;  // in milliseconds
    constexpr int MAX_FRAMESKIP = 10;
    auto next_tick_time = time_point_cast<milliseconds>(steady_clock::now())
        .time_since_epoch()
        .count();

    while (!WindowShouldClose())
    {
        Life::IntVec2 mouseCoords = {
            static_cast<int>(GetMouseX()),
            static_cast<int>(GetMouseY())
        };
        update(g, mouseCoords);

        int loops = 0;
        auto curr_time = time_point_cast<milliseconds>(steady_clock::now())
            .time_since_epoch()
            .count();
        while (curr_time > next_tick_time && loops < MAX_FRAMESKIP)
        {
            if (!g.paused())
                g.advanceTick();
            next_tick_time += tick_wait_interval;
            loops++;
        }

        Life::drawBegin();
        draw(g, mouseCoords);
        debugWindow(g, mouseCoords);
        Life::drawEnd();
    }
}

/*
    to move inside:
    - MAX_FRAMESKIP
    - int loops, curr_time, inner while loop
    - create timing data struct??
*/
void update(Life::Grid& g, const Life::IntVec2& mouse)
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
        g.spawnCell(Life::pxToCellNum(mouse.x), Life::pxToCellNum(mouse.y));
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        g.killCell(Life::pxToCellNum(mouse.x), Life::pxToCellNum(mouse.y));
    if (IsKeyPressed(KEY_ENTER))
        g.advanceTick();
    if (IsKeyPressed(KEY_SPACE))
        g.togglePause();
}

void draw(const Life::Grid& g, const Life::IntVec2& mouse)
{
    Life::drawGrid(g);
    DrawRectangle(
        Life::pxToCellVis(mouse.x), Life::pxToCellVis(mouse.y),
        Life::cellSize, Life::cellSize, DARKBLUE
    );
}
