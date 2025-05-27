#include "imgui.h"
#include "raylib.h"
#include "rlimgui.h"
#include "game-of-life.h"

namespace Life
{

Game::Game() : nextTickTime(0)
{
    InitWindow(SCREEN_W, SCREEN_H, "Game Of Life");
    int refreshRate = GetMonitorRefreshRate(0);
    SetTargetFPS(refreshRate > 144 ? refreshRate/2 : refreshRate);
    rlImGuiSetup(true);
}

Game::Game(std::vector<IntVec2> live) :
    g( std::move(live) ),
    nextTickTime(0)
{
    InitWindow(SCREEN_W, SCREEN_H, "Game Of Life");
    int refreshRate = GetMonitorRefreshRate(0);
    SetTargetFPS(refreshRate > 144 ? refreshRate/2 : refreshRate);
    rlImGuiSetup(true);
}

Game::~Game()
{
    rlImGuiShutdown();
    CloseWindow();
}

void Game::run()
{
    double nextTickTime = GetTime();    // in seconds

    while (!WindowShouldClose())
    {
        options.mouse = {
            static_cast<int>(GetMouseX()),
            static_cast<int>(GetMouseY())
        };
        update();

        drawBegin();
        draw();
        ui();
        drawEnd();
    }
}


void Game::drawBegin()
{
    BeginDrawing();
    rlImGuiBegin();
    ClearBackground(BLACK);
}
void Game::drawEnd()
{
    rlImGuiEnd();
    EndDrawing();
}

void Game::update()
{
    int x = g.pxToCellNum(options.mouse.x);
    int y = g.pxToCellNum(options.mouse.y);
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
        g.spawnCell(x, y);
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        g.killCell(x, y);
    if (IsKeyPressed(KEY_SPACE))
        options.paused = !options.paused;

    if (
        (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) &&
        IsKeyDown(KEY_ENTER)
        )
        g.advanceTick();
    else if (IsKeyPressed(KEY_ENTER))
        g.advanceTick();


    double tickWaitInterval = 1.0 / options.ticksPerSec;  // in seconds
    constexpr int MAX_FRAMESKIP = 10;
    int loops = 0;

    double currTime = GetTime();
    while (currTime > nextTickTime && loops < MAX_FRAMESKIP)
    {
        if (!options.paused)
            g.advanceTick();
        nextTickTime += tickWaitInterval;
        loops++;
    }
}
void Game::draw()
{
    // Draw cells
    if (options.showDetailedState)
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
void Game::ui()
{
    using namespace ImGui;

    int cellX = g.clampX(g.pxToCellNum(options.mouse.x));
    int cellY = g.clampY(g.pxToCellNum(options.mouse.y));
    Begin("Simulation Controls/Settings", NULL);

    Text("FPS: %d", GetFPS());
    NewLine();

    SliderInt("Sim Speed", &(options.ticksPerSec), 1, 10, "%d");
    if (Button("Pause/Play"))
        options.paused = !options.paused;
    SameLine(); Text(options.paused ? "Paused" : "Playing");
    if (Button("Next Tick"))
        g.advanceTick();
    NewLine();

    if (Button("Show Detailed Cell States"))
        options.showDetailedState = !options.showDetailedState;
    SameLine();
    Text(options.showDetailedState ? "On" : "Off");

    if (Button("Toggle Screen Wrapping"))
    {
        options.screenWrap = !options.screenWrap;
        g.setEdgeWrap(options.screenWrap);
    }
    SameLine();
    Text(options.screenWrap ? "On" : "Off");
    NewLine();

    Text("mouse x: %d (%d)", cellX, options.mouse.x);
    Text("mouse y: %d (%d)", cellY, options.mouse.y);
    Text("neighbors: %d", g.neighbors(cellX, cellY));
    End();
}

}