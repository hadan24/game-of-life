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

Game::Game(const std::vector<IntVec2>& live) :
    g(live),
    nextTickTime(0)
{
    InitWindow(SCREEN_W, SCREEN_H, "Game Of Life");
    int refreshRate = GetMonitorRefreshRate(0);
    SetTargetFPS(refreshRate > 144 ? refreshRate/2 : refreshRate);
    rlImGuiSetup(true);
}

Game::Game(std::vector<IntVec2>&& live) :
    g(std::move(live)),
    nextTickTime(0)
{
    InitWindow(SCREEN_W, SCREEN_H, "Game Of Life");
    int refreshRate = GetMonitorRefreshRate(0);
    SetTargetFPS(refreshRate > 144 ? refreshRate / 2 : refreshRate);
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


void Game::drawBegin() const
{
    BeginDrawing();
    rlImGuiBegin();
    ClearBackground(BLACK);
}
void Game::drawEnd() const
{
    rlImGuiEnd();
    EndDrawing();
}

int Game::cellToPx(int cellCoord) const
{
    return cellCoord * cellSize;
}
int Game::pxToCellNum(int pxCoord) const
{
    return pxCoord / cellSize;
}


void Game::update()
{
    int x = pxToCellNum(options.mouse.x);
    int y = pxToCellNum(options.mouse.y);
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
}   // Game::update()

void Game::draw() const
{
    // Draw cells
    if (options.showDetailedState)
    {
        for (int i = 0; i < (g.m_width * g.m_height); i++)
        {
            int y = i / g.m_width;
            int x = i - (y*g.m_width);
            int n = g.neighbors(x, y);

            if (g.isAlive(x, y))
                DrawRectangle(
                    cellToPx(x), cellToPx(y), Life::cellSize, Life::cellSize,
                    n < 2 || n > 3 ? DARKBLUE : BLUE    // will die : stay alive
                );
            else if (n == 3)    // not alive but will come alive
                DrawRectangle(
                    cellToPx(x), cellToPx(y),
                    Life::cellSize, Life::cellSize, SKYBLUE
                );
        }
    }
    else
    {
        for (int i = 0; i < (g.m_width * g.m_height); i++)
        {
            int y = i / g.m_width;
            int x = i - (y*g.m_width);
            if (g.isAlive(x, y))
                DrawRectangle(
                    cellToPx(x), cellToPx(y),
                    Life::cellSize, Life::cellSize, BLUE
                );
        }
    }

    // Draw grid lines
    for (int i = 0; i < Life::SCREEN_W; i++)
        DrawLine(cellToPx(i), 0, cellToPx(i), Life::SCREEN_H, DARKGRAY);
    for (int j = 0; j < Life::SCREEN_H; j++)
        DrawLine(0, cellToPx(j), Life::SCREEN_W, cellToPx(j), DARKGRAY);

    // Highlight moused cell
    DrawRectangle(
        static_cast<int>(options.mouse.x / Life::cellSize) * Life::cellSize,
        static_cast<int>(options.mouse.y / Life::cellSize) * Life::cellSize,
        Life::cellSize, Life::cellSize, LIGHTGRAY
    );
}   // Game::draw()

void Game::ui()
{
    using namespace ImGui;
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

    int cellX = pxToCellNum(options.mouse.x);
    int cellY = pxToCellNum(options.mouse.y);
    Text("mouse x: %d (%d)", cellX, options.mouse.x);
    Text("mouse y: %d (%d)", cellY, options.mouse.y);
    int n = g.neighbors(cellX, cellY);
    if (n == -1)
        Text("neighbors: [invalid cell]");
    else
        Text("neighbors: %d", n);
    End();
}   // Game::ui()

}   // namespace Life