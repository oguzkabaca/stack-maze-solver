/**
 * @file    mazeGen.cpp
 * @brief   Bit duzeyinde labirent modelleme prototipi (projeden once yapilan
 *          bagimsiz calisma). Her hucre 4 bitlik duvar maskesi (N/E/S/W) ile
 *          temsil edilir. Projedeki src/LabirentUretici.cpp bu modelden
 *          uyarlanmistir. Makefile tarafindan derlenmez; tek basina denemek icin:
 *              g++ -o mazeGen prototip/mazeGen.cpp
 */
#include <cstdint>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

struct Neighbor {
    int row;
    int col;
    uint8_t direction;
};

struct Address {
    int row;
    int col;
};

constexpr uint8_t NORTH = 0b1000;
constexpr uint8_t EAST = 0b0100;
constexpr uint8_t SOUTH = 0b0010;
constexpr uint8_t WEST = 0b0001;

uint8_t getOppositeWall(uint8_t targetWall) {
    switch (targetWall) {
        case NORTH:
            return SOUTH;
        case SOUTH:
            return NORTH;
        case EAST:
            return WEST;
        case WEST:
            return EAST;
        default:
            return 0;
    }
}

bool removeWall(uint8_t& currentCell, uint8_t& neighborCell, uint8_t targetWall) {
    uint8_t oppositeWall = getOppositeWall(targetWall);

    bool wallA = currentCell & targetWall;
    bool wallB = neighborCell & oppositeWall;

    if (!wallA && !wallB) {
        return false;
    }
    else if (wallA && wallB) {
        currentCell &= ~targetWall;
        neighborCell &= ~oppositeWall;
        return true;
    }
    else {
        return false;
    }
}

bool isInside(int mazeSize, int row, int col) {
    return row >= 0 &&
           row < mazeSize &&
           col >= 0 &&
           col < mazeSize;
}

vector<Neighbor> getUnvisitedNeighbors(
    int row,
    int col,
    int mazeSize,
    const vector<vector<bool>>& visited
) {
    vector<Neighbor> neighbors;

    int newRow = row - 1;
    int newCol = col;

    if (isInside(mazeSize, newRow, newCol) && !visited[newRow][newCol]) {
        neighbors.push_back({newRow, newCol, NORTH});
    }

    newRow = row;
    newCol = col + 1;

    if (isInside(mazeSize, newRow, newCol) && !visited[newRow][newCol]) {
        neighbors.push_back({newRow, newCol, EAST});
    }

    newRow = row + 1;
    newCol = col;

    if (isInside(mazeSize, newRow, newCol) && !visited[newRow][newCol]) {
        neighbors.push_back({newRow, newCol, SOUTH});
    }

    newRow = row;
    newCol = col - 1;

    if (isInside(mazeSize, newRow, newCol) && !visited[newRow][newCol]) {
        neighbors.push_back({newRow, newCol, WEST});
    }

    return neighbors;
}

Neighbor selectRandomNeighbor(
    const vector<Neighbor>& neighbors,
    mt19937& generator
) {
    uniform_int_distribution<size_t> distribution(0, neighbors.size() - 1);
    size_t randomIndex = distribution(generator);

    return neighbors[randomIndex];
}

Address selectRandomBaseCell(int mazeSize, mt19937& generator) {
    int legalPlaces = 4 * mazeSize - 4;

    uniform_int_distribution<int> distribution(0, legalPlaces - 1);
    int index = distribution(generator);

    if (index < mazeSize) {
        return {0, index};
    }

    index -= mazeSize;

    if (index < mazeSize - 2) {
        return {index + 1, mazeSize - 1};
    }

    index -= mazeSize - 2;

    if (index < mazeSize) {
        return {mazeSize - 1, mazeSize - 1 - index};
    }

    index -= mazeSize;

    return {mazeSize - 2 - index, 0};
}

int main() {
    int mazeSize = 10;

    random_device rd;
    mt19937 generator(rd());

    vector<vector<uint8_t>> maze(
        mazeSize,
        vector<uint8_t>(mazeSize, 0b1111)
    );

    vector<vector<bool>> visited(
        mazeSize,
        vector<bool>(mazeSize, false)
    );

    Address baseCell = selectRandomBaseCell(mazeSize, generator);

    cout << "Base cell: "
         << baseCell.row << ", "
         << baseCell.col << '\n';

    return 0;
}