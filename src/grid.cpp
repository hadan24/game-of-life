#include "game-of-life.h"
#include <array>

// up, up-right, right, down-right, down, etc
static const std::array<Life::IntVec2, 8> dirs(
    { {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1} }
);

namespace Life
{

Grid::Grid() :
    m_data( std::move(std::vector<char>(m_width*m_height, CellState::DEAD)) ),
    m_neighbors( std::move(std::vector<char>(m_width*m_height, 0)) )
{}

Grid::Grid(const std::vector<IntVec2>& live) :
    m_data( std::move(std::vector<char>(m_width* m_height, CellState::DEAD)) ),
    m_neighbors( std::move(std::vector<char>(m_width*m_height, 0)) )
{
    for (auto& c : live)
        spawnCell(c.x, c.y);
}

Grid::Grid(std::vector<IntVec2>&& live) :
    m_data(std::move(std::vector<char>(m_width* m_height, CellState::DEAD))),
    m_neighbors(std::move(std::vector<char>(m_width* m_height, 0)))
{
    for (auto& c : live)
        spawnCell(c.x, c.y);
}

bool Grid::isAlive(int x, int y) const
{
    if (!inBounds(x, y))
        return false;
    return m_data[x + y*m_width] == CellState::ALIVE;
}

void Grid::flipCell(int x, int y)
{
    if (!inBounds(x, y))
        return;

    if (m_data[x + y*m_width] == CellState::ALIVE)
        killCell(x, y);
    else spawnCell(x, y);
}

void Grid::spawnCell(int x, int y)
{
    int cell = x + y*m_width;
    if (!inBounds(x, y) || m_data[cell] == CellState::ALIVE)
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

void Grid::killCell(int x, int y)
{
    int cell = x + y*m_width;
    if (!inBounds(x, y) || m_data[cell] == CellState::DEAD)
        return;

    m_data[cell] = CellState::DEAD;
    for (auto& d : dirs)
    {
        if (inBounds(x+d.x, y+d.y))
            m_neighbors[cell + d.x + d.y*m_width]--;
        else if (edgeWrap)
        {
            int tempX = wrapX(x + d.x);
            int tempY = wrapY(y + d.y);
            m_neighbors[tempX + tempY*m_width]--;
        }
    }
}

void Grid::advanceTick()
{
    for (int i = 0; i < m_neighbors.size(); i++)
    {
        if (m_neighbors[i] == 3 || (m_data[i] == CellState::ALIVE && m_neighbors[i] == 2))
            m_data[i] = CellState::ALIVE;
        else m_data[i] = CellState::DEAD;
    }

    for (int i = 0; i < m_data.size(); i++)
    {
        int y = i / m_width;
        int x = i - (y*m_width);

        m_neighbors[i] = 0;
        for (auto& d : dirs)
        {
            if (
                inBounds(x+d.x, y+d.y) &&
                m_data[x+d.x + (y+d.y) * m_width] == CellState::ALIVE
            )
                m_neighbors[i]++;
            else if (
                !inBounds(x+d.x, y+d.y) &&
                edgeWrap &&
                m_data[wrapX(x+d.x) + (wrapY(y+d.y)*m_width)] == CellState::ALIVE
            )
                m_neighbors[i]++;
        }
    }
}   // Grid::advanceTick()

int Grid::neighbors(int x, int y) const
{
    if (!inBounds(x, y))
        return -1;
    return m_neighbors[x + y*m_width];
}

void Grid::setEdgeWrap(bool val)
{
    edgeWrap = val;

    int lastRow = (m_height - 1) * m_width;
    for (int x = 0; x < m_width; x++)
    {
        for (int dx : {-1, 0, 1})
        {
            int currNeighborX = wrapX(x + dx);
            if (m_data[currNeighborX + lastRow] == CellState::ALIVE)
                m_neighbors[x] += (edgeWrap ? 1 : -1);  // top row, check last
            if (m_data[currNeighborX] == CellState::ALIVE)
                m_neighbors[x + lastRow] += (edgeWrap ? 1 : -1);    // bot, check top
        }
    }
    int lastCol = m_width - 1;
    for (int y = 0; y < m_height; y++)
    {
        int currY = y * m_width;
        for (int dy : {-1, 0, 1})
        {
            int currNeighborY = wrapY(y+dy) * m_width;
            if (m_data[lastCol + currNeighborY] == CellState::ALIVE)
                m_neighbors[currY] += (edgeWrap ? 1 : -1);  // left col, check right
            if (m_data[currNeighborY] == CellState::ALIVE)
                m_neighbors[lastCol + currY] += (edgeWrap ? 1 : -1);    // right, check left
        }
    }
}   // Grid::setEdgeWrap


bool Grid::inBounds(int x, int y) const
{
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

int Grid::wrapX(int x) const
{
    if (x < 0)
        return m_width + x;
    if (x >= m_width)
        return x - m_width;
    return x;
}

int Grid::wrapY(int y) const
{
    if (y < 0)
        return m_height + y;
    if (y >= m_height)
        return y - m_height;
    return y;
}

}   // namespace Life