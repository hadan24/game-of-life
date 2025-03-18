#include "grid-bits.h"
#include <array>
#include <iostream>

// up, up-right, right, down-right, down, etc
static std::array<Life::IntVec2, 8> dirs(
    { {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1} }
);
static unsigned char liveBit = 1 << 4;


GridBits::GridBits()
    : m_data(std::move(std::vector<unsigned char>(width*height, CellState::DEAD))) {}

bool GridBits::checkCell(int x, int y) const {
    return (m_data[x + y*width] & liveBit) == CellState::ALIVE;
}
unsigned char GridBits::getCell(int x, int y) const {
    return m_data[x + y*width];
}

void GridBits::flipCell(int x, int y) {
    if (checkCell(x, y))
        killCell(x, y);
    else spawnCell(x, y);
}

void GridBits::spawnCell(int x, int y) {
    if (!checkCell(x, y)) {
        for (auto& d : dirs) {
            int currX = x + d.x;
            int currY = (y + d.y) * width;
            if (
                (currX + currY < 0 || currX + currY >= m_data.size()) ||    // y edge
                ((x % width == 0 && d.x == -1) || (x % width == width - 1 && d.x == 1)) // x edge
            )
                continue;
            m_data[currX + currY]++;
        }
    }
    m_data[x + y*width] |= liveBit;
}

void GridBits::killCell(int x, int y) {
    if (checkCell(x, y)) {
        for (auto& d : dirs) {
            int currX = x + d.x;
            int currY = (y + d.y) * width;
            if (
                (currX + currY < 0 || currX + currY >= m_data.size()) ||    // y edge
                ((x % width == 0 && d.x == -1) || (x % width == width - 1 && d.x == 1)) // x edge
            )
                continue;
            m_data[x+d.x + (y+d.y)*width]--;
        }
    }
    m_data[x + y*width] &= (~liveBit);
}

void GridBits::advTick() {
    for (int i = 0; i < m_data.size(); i++) {
        int neighbors = m_data[i] & (~liveBit);
        int state = m_data[i] & liveBit;

        if ( neighbors == 3 || (state == CellState::ALIVE && neighbors == 2) )
            m_data[i] = CellState::ALIVE;
        else m_data[i] = CellState::DEAD;
    }

    for (int i = 0; i < m_data.size(); i++) {
        for (auto& d : dirs) {
            bool atYEdge = (i + d.y*width < 0) || (i + d.y*width >= m_data.size());
            bool atXEdge = (i%width == 0 && d.x == -1) || (i%width == width-1 && d.x == 1);
            if (atYEdge || atXEdge)
                continue;
            else if ( (m_data[i + d.x + d.y * width] & liveBit) == CellState::ALIVE )
                m_data[i]++;
        }
    }
}

void GridBits::advanceTicks(int numTicks) {
    for (int i = 0; i < numTicks; i++)
        advTick();
}


void GridBits::print() const {
    using std::cout, std::endl;

    for (int i = 0; i < m_data.size(); i++) {
        if (i % width == 0)
            cout << endl;
        cout << (int)m_data[i] << " ";
    }
}