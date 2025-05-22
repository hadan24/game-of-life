#include "game-of-life.h"

namespace Life
{
    void gameSetup()
    {
        InitWindow(SCREEN_W, SCREEN_H, "Game Of Life");
        SetTargetFPS(144);
        rlImGuiSetup(true);
    }
    void gameTeardown()
    {
        rlImGuiShutdown();
        CloseWindow();
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


    void runGame()
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

        Life::UIData ui = {};
        double nextTickTime = GetTime();

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

    void update(Life::Grid& g, Life::UIData& uiData, double& nextTickTime)
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
            g.spawnCell(g.pxToCellNum(uiData.mouse.x), g.pxToCellNum(uiData.mouse.y));
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            g.killCell(g.pxToCellNum(uiData.mouse.x), g.pxToCellNum(uiData.mouse.y));
        if (IsKeyPressed(KEY_SPACE))
            uiData.paused = !uiData.paused;

        if (
            (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) &&
            IsKeyDown(KEY_ENTER)
        )
            g.advanceTick();
        else if (IsKeyPressed(KEY_ENTER))
            g.advanceTick();


        double tickWaitInterval = 1.0 / uiData.ticksPerSec;  // in milliseconds
        constexpr int MAX_FRAMESKIP = 10;
        int loops = 0;

        double currTime = GetTime();
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
                            g.cellToPx(i), g.cellToPx(j),
                            Life::cellSize, Life::cellSize,
                            n < 2 || n > 3 ? DARKBLUE : BLUE    // will die : stay alive
                        );
                    else if (n == 3)    // not alive but will come alive
                        DrawRectangle(
                            g.cellToPx(i), g.cellToPx(j),
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
                            g.cellToPx(i), g.cellToPx(j),
                            Life::cellSize, Life::cellSize, BLUE
                        );
        }

        // Draw grid lines
        for (int i = 0; i < Life::SCREEN_W; i++)
            DrawLine(g.cellToPx(i), 0, g.cellToPx(i), Life::SCREEN_H, DARKGRAY);
        for (int j = 0; j < Life::SCREEN_H; j++)
            DrawLine(0, g.cellToPx(j), Life::SCREEN_W, g.cellToPx(j), DARKGRAY);

        // Highlight moused cell
        DrawRectangle(
            g.pxToCellVis(options.mouse.x),
            g.pxToCellVis(options.mouse.y),
            Life::cellSize, Life::cellSize, LIGHTGRAY
        );
    }
    void uiWindow(Life::Grid& g, Life::UIData& ui) {
        using namespace ImGui;

        int cellX = g.clampX(g.pxToCellNum(ui.mouse.x));
        int cellY = g.clampY(g.pxToCellNum(ui.mouse.y));
        Begin("Simulation Controls/Settings", NULL);

        Text("FPS: %d", GetFPS());
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