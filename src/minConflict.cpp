#include "minConflict.h"
#include <cstdlib>
#include <utility>
#include <iostream>

std::vector<std::vector<int>> generateMap(int minValue, int maxValue, int size)
{
    std::vector<std::vector<int>> map(size, std::vector<int>(size, 0));
    // Implement your map generation logic here using the min-conflicts algorithm
    // For demonstration purposes, we'll fill the map with random values between minValue and maxValue
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            map[i][j] = minValue + rand() % (maxValue - minValue + 1);
        }
    }
    return map;
}
/**
 * @brief Function to count the number of conflicts for a given tile value at position (x, y) in the map. 
 *      A conflict occurs when the newValue differs from its neighbors by more than 1. 
 *      The newValue, could be a curently assigned value or a potential value we want to assign to the tile.
 *      The function checks both orthogonal and diagonal neighbors based on the diagonal flag.
 * 
 * @param map 2D array of intages representing the map, where each integer corresponds to a terrain type.
 * @param x X-coordinate of the tile.
 * @param y Y-coordinate of the tile.
 * @param newValue Potential value to assign to the tile.
 * @param diagonal Flag indicating whether to check diagonal neighbors.
 * @return int Number of conflicts.
 */
int countConflicts(const std::vector<std::vector<int>>& map, int x, int y, int newValue, bool diagonal)
{
    int conflicts = 0;
    int size = map.size();

    // Check the four orthogonal neighbors
    if (x > 0 && abs(map[x - 1][y] -newValue)>1) conflicts++;
    if (x < size - 1 && abs(map[x + 1][y] -newValue)>1) conflicts++;
    if (y > 0 && abs(map[x][y - 1] -newValue)>1) conflicts++;
    if (y < size - 1 && abs(map[x][y + 1] -newValue)>1) conflicts++;

    // Optionally, check diagonal neighbors if needed
    if (diagonal) {
        if (x > 0 && y > 0 && abs(map[x - 1][y - 1] -newValue)>1) conflicts++;
        if (x > 0 && y < size - 1 && abs(map[x - 1][y + 1] -newValue)>1) conflicts++;
        if (x < size - 1 && y > 0 && abs(map[x + 1][y - 1] -newValue)>1) conflicts++;
        if (x < size - 1 && y < size - 1 && abs(map[x + 1][y + 1] -newValue)>1) conflicts++;
    }

    return conflicts;
}

bool lowerConflictValue(std::vector<std::vector<int>>& map, int x, int y, int minValue, int maxValue, bool diagonal)
{
    bool madeLower=false;
    int curentValue = map[x][y];
    int curentConflicts = countConflicts(map, x, y, curentValue, diagonal);
    int lowestConflicts = curentConflicts;
    int bestValue = curentValue;

    for (int value = minValue; value <= maxValue; ++value) {
        int conflicts = countConflicts(map, x, y, value, diagonal);
            if (conflicts < lowestConflicts) {
                lowestConflicts = conflicts;
                bestValue = value;
                madeLower=true;
        }
    }
    map[x][y] = bestValue;

    return madeLower;
}
int countTotalConflicts(const std::vector<std::vector<int>>& map, bool diagonal)
{
    int totalConflicts = 0;
    int size = map.size();

    for (int x = 0; x < size; ++x) {
        for (int y = 0; y < size; ++y) {
            totalConflicts += countConflicts(map, x, y, map[x][y], diagonal);
        }
    }

    return totalConflicts;
}

int resolveConflictOnePass(std::vector<std::vector<int>>& map, int minValue, int maxValue, bool diagonal)
{
    int totalConflicts = 0;
    // Find the cells with the highest conflict count.
    int maxConflicts = -1;
    std::vector<std::pair<int, int>> worstCells;

    for (std::size_t x = 0; x < map.size(); ++x) {
        for (std::size_t y = 0; y < map[x].size(); ++y) {
            int conflicts = countConflicts(map, static_cast<int>(x), static_cast<int>(y), map[x][y], diagonal);
            totalConflicts += conflicts;

            if (conflicts > maxConflicts) {
                //we found a new maximum, remove cels for the old one and add this one.
                maxConflicts = conflicts;
                worstCells.clear();
                worstCells.push_back({static_cast<int>(x), static_cast<int>(y)});
            } else if (conflicts == maxConflicts && conflicts > 0) {
                worstCells.push_back({static_cast<int>(x), static_cast<int>(y)});
            }
        }
    }

    if (maxConflicts <= 0 || worstCells.empty()) {
        return totalConflicts; // No conflicts to resolve
    }

    // Go throught the worst cells and try to lower their conflict count for every cell
    //untill we find one that can be improved. If we find one, we return immediately to start a new pass with the updated map.
    for (const auto& [x, y] : worstCells) {
        if (lowerConflictValue(map, x, y, minValue, maxValue, diagonal)) {
            return totalConflicts;
        }
    }
//std::cout << "Stuck at " << maxConflicts << " conflicts, trying to perturb the map..." << std::endl;
    // If none of the worst cells can be improved, perturb one of them to escape the local minimum.
    const std::size_t randomIndex = static_cast<std::size_t>(rand()) % worstCells.size();
    const auto& [x, y] = worstCells[randomIndex];
    map[x][y] = minValue + rand() % (maxValue - minValue + 1);//does not work on diagonals if numTerains >5
    //change its neighbors to the cell value
    makeNeighborValuesSame(map, x, y, diagonal);//did not work along. Worsth then just changing the cell value. 
    //but in combination with setting one "bad" cel to the random values, it seems to help escape the local minimum.
    return totalConflicts;
}
void makeNeighborValuesSame(std::vector<std::vector<int>>& map, int x, int y, bool diagonal)
{
    int cellValue = map[x][y];
    if (x > 0) map[x - 1][y] = cellValue;
    if (x < map.size() - 1) map[x + 1][y] = cellValue;
    if (y > 0) map[x][y - 1] = cellValue;
    if (y < map[x].size() - 1) map[x][y + 1] = cellValue;
    if (diagonal) {
        if (x > 0 && y > 0) map[x - 1][y - 1] = cellValue;
        if (x > 0 && y < map[x].size() - 1) map[x - 1][y + 1] = cellValue;
        if (x < map.size() - 1 && y > 0) map[x + 1][y - 1] = cellValue;
        if (x < map.size() - 1 && y < map[x].size() - 1) map[x + 1][y + 1] = cellValue;
    }
}