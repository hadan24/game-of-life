#include "grid.h"

Life::Grid::Grid() : m_data(std::vector<char>(m*n, false)) {}

bool Life::Grid::getCell(int x, int y) {
    return m_data[x + y*m];
}

void Life::Grid::flipCell(int x, int y) {
    m_data[x + y*m] = m_data[x + y*m] ? false : true;
}

void Life::Grid::spawnCell(int x, int y) {
    m_data[x + y*m] = true;
}

void Life::Grid::killCell(int x, int y) {
    m_data[x + y*m] = false;
}

void Life::Grid::advanceTicks(int numTicks) {
    for (int i = 0; i < numTicks; i++) {
        
    }
}
