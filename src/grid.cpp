#include "game-of-life.h"
#include <array>

// up, up-right, right, down-right, down, etc
static const std::array<Life::IntVec2, 8> dirs(
    { {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1} }
);


Life::Grid::Grid() :
    m_data( std::move(std::vector<char>(m_width*m_height, CellState::DEAD)) ),
    m_neighbors( std::move(std::vector<char>(m_width*m_height, 0)) )
{}

Life::Grid::Grid(std::vector<IntVec2> live) :
    m_data( std::move(std::vector<char>(m_width* m_height, CellState::DEAD)) ),
    m_neighbors( std::move(std::vector<char>(m_width* m_height, 0)) )
{
    for (auto& c : live)
        spawnCell(c.x, c.y);
}

bool Life::Grid::isAlive(int x, int y) const {
    return m_data[x + y*m_width] == CellState::ALIVE;
}

void Life::Grid::flipCell(int x, int y)
{
    if (m_data[x + y*m_width] == CellState::ALIVE)
        killCell(x, y);
    else spawnCell(x, y);
}

void Life::Grid::spawnCell(int x, int y)
{
    int cell = x + y*m_width;
    if (m_data[cell] == CellState::ALIVE)
        return;

    m_data[cell] = CellState::ALIVE;
    for (auto& d : dirs)
    {
        if (inBounds(x+d.x, y+d.y))
            m_neighbors[cell + d.x + d.y*m_width]++;
        else if (edgeWrap)
        {
            int tempX = wrapX(x + d.x);
            int tempY = wrapY(y + d.y);
            m_neighbors[tempX + tempY*m_width]++;
        }
    }
}

void Life::Grid::killCell(int x, int y)
{
    int cell = x + y*m_width;
    if (m_data[cell] == CellState::DEAD)
        return;

    m_data[cell] = CellState::DEAD;
    for (auto& d : dirs)
    {
        if (inBounds(x + d.x, y + d.y))
            m_neighbors[cell + d.x + d.y*m_width]--;
        else if (edgeWrap)
        {
            int tempX = wrapX(x + d.x);
            int tempY = wrapY(y + d.y);
            m_neighbors[tempX + tempY * m_width]++;
        }
    }
}

void Life::Grid::advanceTick()
{
    for (int i = 0; i < m_neighbors.size(); i++)
    {
        if (m_neighbors[i] == 3 || (m_data[i] == CellState::ALIVE && m_neighbors[i] == 2))
            m_data[i] = CellState::ALIVE;
        else m_data[i] = CellState::DEAD;
    }

    for (int y = 0; y < m_height; y++)
    {
        int col = y*m_width;
        for (int x = 0; x < m_width; x++)
        {
            m_neighbors[x + col] = 0;

            for (auto& d : dirs)
            {
                if (
                    inBounds(x+d.x, y+d.y) &&
                    m_data[x+d.x + (y+d.y)*m_width] == CellState::ALIVE
                )
                    m_neighbors[x + col]++;
                else if (
                    !inBounds(x+d.x, y+d.y) &&
                    edgeWrap &&
                    m_data[wrapX(x+d.x) + (wrapY(y+d.y)*m_width)] == CellState::ALIVE
                )
                    m_neighbors[x + col]++;
            }
        }
    }
}

int Life::Grid::neighbors(int x, int y) const
{
    return m_neighbors[x + y*m_width];
}

void Life::Grid::setEdgeWrap(bool val)
{
    edgeWrap = val;
}


bool Life::Grid::inBounds(int x, int y) const
{
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

int Life::Grid::wrapX(int x) const
{
    if (x < 0)
        return m_width + x;
    if (x >= m_width)
        return x - m_width;
    return x;
}

int Life::Grid::wrapY(int y) const
{
    if (y < 0)
        return m_height + y;
    if (y >= m_height)
        return y - m_height;
    return y;
}