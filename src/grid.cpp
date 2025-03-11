#include "game-of-life.h"
#include <array>
using size_type = std::vector<char>::size_type;


Life::Grid::Grid() : m_data( std::move(std::vector<char>(width*height, false)) ) {}

bool Life::Grid::getCell(size_type x, size_type y) const {
    return m_data[x + y*width];
}

void Life::Grid::flipCell(size_type x, size_type y) {
    m_data[x + y*width] = m_data[x + y*width] ? false : true;
}

void Life::Grid::spawnCell(size_type x, size_type y) { m_data[x + y*width] = true; }

void Life::Grid::killCell(size_type x, size_type y) { m_data[x + y*width] = false; }

void Life::Grid::advTick() {
    // up, up-right, right, down-right, down, etc
    std::array<IntVec2, 8> dirs (
        { {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,1} }
    );

    for (int i = 0; i < width*height; i++) {
        int aliveNeighbors = 0;
        for (auto& d : dirs) {
            bool atYEdge = (i + d.y*width < 0) || (i + d.y*width >= m_data.size());
            bool atXEdge = (i%width == 0 && d.x == -1) || (i%width == width-1 && d.x == 1);
            if (atYEdge || atXEdge)
                continue;
            else if (m_data[i+d.x + d.y*width])
                aliveNeighbors++;
        }

        if (aliveNeighbors == 3)    // cell comes/stays alive, guaranteed
            m_data[i] = true;
        else if (!m_data[i] || aliveNeighbors == 2)
            continue;   // cell stays same state, whether dead or alive
        else
            m_data[i] = false;  // cell dies otherwise
    }
}

void Life::Grid::advanceTicks(int numTicks) {   // naive direct implementation
    for (int i = 0; i < numTicks; i++)
        advTick();
}
