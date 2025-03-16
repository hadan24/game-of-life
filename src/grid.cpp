#include "game-of-life.h"
#include <array>


Life::Grid::Grid() 
    : m_data( std::move(std::vector<char>(width*height, CellState::DEAD)) ) {}

bool Life::Grid::checkCell(int x, int y) const {
    return m_data[x + y*width] == CellState::ALIVE;
}

void Life::Grid::flipCell(int x, int y) {
    m_data[x + y*width] = 
        m_data[x + y*width] == CellState::ALIVE ? false : true;
}

void Life::Grid::spawnCell(int x, int y) {
    m_data[x + y*width] = CellState::ALIVE;
}

void Life::Grid::killCell(int x, int y) {
    m_data[x + y*width] = CellState::DEAD;
}

void Life::Grid::advTick() {
    // up, up-right, right, down-right, down, etc
    std::array<IntVec2, 8> dirs (
        { {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1} }
    );

    std::vector<char> liveNeighbors(width*height, 0);

    for (int i = 0; i < m_data.size(); i++) {
        for (auto& d : dirs) {
            bool atYEdge = (i + d.y*width < 0) || (i + d.y*width >= m_data.size());
            bool atXEdge = (i%width == 0 && d.x == -1) || (i%width == width-1 && d.x == 1);
            if (atYEdge || atXEdge)
                continue;
            else if (m_data[i+d.x + d.y*width] == CellState::ALIVE)
                liveNeighbors[i]++;
        }
    }
    for (int i = 0; i < liveNeighbors.size(); i++) {
        if (liveNeighbors[i] == 3)  // cell comes/stays alive, guaranteed
            m_data[i] = CellState::ALIVE;
        else if (m_data[i] == CellState::DEAD || liveNeighbors[i] == 2)
            continue;   // cell stays same state, whether dead or alive
        else
            m_data[i] = CellState::DEAD;    // cell dies otherwise
    }
}

void Life::Grid::advanceTicks(int numTicks) {   // naive direct implementation
    for (int i = 0; i < numTicks; i++)
        advTick();
}
