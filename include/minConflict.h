#ifndef MINCONFLICT_H
#define MINCONFLICT_H   

#include <vector>
std::vector<std::vector<int>> generateMap(int minValue, int maxValue, int size);
int countConflicts(const std::vector<std::vector<int>>& map, int x, int y, int newValue, bool diagonal = false);
bool lowerConflictValue(std::vector<std::vector<int>>& map, int x, int y, int minValue, int maxValue, bool diagonal = false);
int countTotalConflicts(const std::vector<std::vector<int>>& map, bool diagonal = false);
void resolveConflictOnePass(std::vector<std::vector<int>>& map, int minValue, int maxValue, bool diagonal = false);
#endif