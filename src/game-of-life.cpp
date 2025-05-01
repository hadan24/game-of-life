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
            Life::update(g, ui, nextTickTime);


            Life::drawBegin();
            Life::draw(g, ui);
            Life::uiWindow(g, ui);
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
        if (IsKeyPressed(KEY_SPACE))
            uiData.paused = !uiData.paused;

        if (
            (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) &&
            IsKeyDown(KEY_ENTER)
        )
            g.advanceTick();
        else if (IsKeyPressed(KEY_ENTER))
            g.advanceTick();


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
    void draw(const Life::Grid& g, const Life::UIData& options)
    {
        // Draw cells
        if (options.showDetailedCellState)
        {
            for (int i = 0; i < g.m_width; i++)
            {
                for (int j = 0; j < g.m_height; j++)
                {
                    int n = g.neighbors(i, j);
                    if (g.isAlive(i, j))
                        DrawRectangle(
                            Life::cellToPx(i), Life::cellToPx(j),
                            Life::cellSize, Life::cellSize,
                            n < 2 || n > 3 ? DARKBLUE : BLUE    // will die : stay alive
                        );
                    else if (n == 3)    // not alive but will come alive
                        DrawRectangle(
                            Life::cellToPx(i), Life::cellToPx(j),
                            Life::cellSize, Life::cellSize, SKYBLUE
                        );
                }
            }
        }
        else
        {
            for (int i = 0; i < g.m_width; i++)
                for (int j = 0; j < g.m_height; j++)
                    if (g.isAlive(i, j))
                        DrawRectangle(
                            Life::cellToPx(i), Life::cellToPx(j),
                            Life::cellSize, Life::cellSize, BLUE
                        );
        }

        // Draw grid lines
        for (int i = 0; i < Life::SCREEN_W; i++)
            DrawLine(Life::cellToPx(i), 0, Life::cellToPx(i), Life::SCREEN_H, DARKGRAY);
        for (int j = 0; j < Life::SCREEN_H; j++)
            DrawLine(0, Life::cellToPx(j), Life::SCREEN_W, Life::cellToPx(j), DARKGRAY);

        // Highlight moused cell
        DrawRectangle(
            Life::pxToCellVis(options.mouse.x),
            Life::pxToCellVis(options.mouse.y),
            Life::cellSize, Life::cellSize, LIGHTGRAY
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

        if (Button("Toggle Screen Wrapping"))
        {
            ui.screenWrap = !ui.screenWrap;
            g.setEdgeWrap(ui.screenWrap);
        }
        SameLine();
        Text(ui.screenWrap ? "On" : "Off");
        NewLine();

        Text("mouse x: %d (%d)", cellX, ui.mouse.x);
        Text("mouse y: %d (%d)", cellY, ui.mouse.y);
        Text("neighbors: %d", g.neighbors(cellX, cellY));
        End();
    }
}