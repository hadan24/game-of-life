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

    std::vector<char> neighbors(width*height, 0);

    for (int i = 0; i < m_data.size(); i++) {
        for (auto& d : dirs) {
            bool atYEdge = (i + d.y*width < 0) || (i + d.y*width >= m_data.size());
            bool atXEdge = (i%width == 0 && d.x == -1) || (i%width == width-1 && d.x == 1);
            if (atYEdge || atXEdge)
                continue;
            else if (m_data[i+d.x + d.y*width] == CellState::ALIVE)
                neighbors[i]++;
        }
    }
    for (int i = 0; i < neighbors.size(); i++) {
        if ( neighbors[i] == 3 || (m_data[i] == CellState::ALIVE && neighbors[i] == 2) )
            m_data[i] = CellState::ALIVE;
        else m_data[i] = CellState::DEAD;
    }
}

void Life::Grid::advanceTicks(int numTicks) {   // naive direct implementation
    for (int i = 0; i < numTicks; i++)
        advTick();
}
