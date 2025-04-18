#include "game-of-life.h"
#include <array>

// up, up-right, right, down-right, down, etc
static std::array<Life::IntVec2, 8> dirs(
    { {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1} }
);


Life::Grid::Grid() :
    m_data( std::move(std::vector<char>(width*height, CellState::DEAD)) ),
    m_neighbors( std::move(std::vector<char>(width*height, 0)) )
{}

Life::Grid::Grid(std::vector<IntVec2> live) :
    m_data( std::move(std::vector<char>(width* height, CellState::DEAD)) ),
    m_neighbors( std::move(std::vector<char>(width* height, 0)) )
{
    for (auto& c : live)
        spawnCell(c.x, c.y);
}

bool Life::Grid::isAlive(int x, int y) const {
    return m_data[x + y*width] == CellState::ALIVE;
}

void Life::Grid::flipCell(int x, int y)
{
    int cell = x + y*width;
    if (m_data[cell] == CellState::ALIVE)
    {
        m_data[cell] = CellState::DEAD;
        for (auto& d : dirs)
            m_neighbors[cell+d.x + d.y*width]--;
    }
    else
    {
        m_data[cell] = CellState::ALIVE;
        for (auto& d : dirs)
            m_neighbors[cell+d.x + d.y*width]++;
    }
}

void Life::Grid::spawnCell(int x, int y)
{
    int cell = x + y*width;
    if (m_data[cell] == CellState::ALIVE)
        return;

    m_data[cell] = CellState::ALIVE;
    for (auto& d : dirs)
        m_neighbors[cell + d.x + d.y * width]++;
}

void Life::Grid::killCell(int x, int y)
{
    int cell = x + y*width;
    if (m_data[cell] == CellState::DEAD)
        return;

    m_data[cell] = CellState::DEAD;
    for (auto& d : dirs)
        m_neighbors[cell + d.x + d.y * width]--;
}

void Life::Grid::advanceTick()
{
    for (int i = 0; i < m_neighbors.size(); i++) {
        if (m_neighbors[i] == 3 || (m_data[i] == CellState::ALIVE && m_neighbors[i] == 2))
            m_data[i] = CellState::ALIVE;
        else m_data[i] = CellState::DEAD;
    }

    for (int i = 0; i < m_data.size(); i++) {
        m_neighbors[i] = 0;

        for (auto& d : dirs) {
            bool atYEdge = (i + d.y * width < 0) || (i + d.y * width >= m_data.size());
            bool atXEdge = (i % width == 0 && d.x == -1) || (i % width == width - 1 && d.x == 1);
            if (atYEdge || atXEdge)
                continue;
            else if (m_data[i + d.x + d.y * width] == CellState::ALIVE)
                m_neighbors[i]++;
        }
    }
}

void Life::Grid::togglePause()
{
    m_paused = !m_paused;
}

bool Life::Grid::paused() const
{
    return m_paused;
}