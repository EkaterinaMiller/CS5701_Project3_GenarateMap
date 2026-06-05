#ifndef HELPERS_H
#define HELPERS_H
#include <vector>
#include <string>
std::string vectorToString(const std::vector<std::string>& vec) {
    std::string result;
    for (const auto& item : vec) {
        result += item + ", ";
    }
    // Remove the trailing comma and space
    if (!result.empty()) {
        result.pop_back();
        result.pop_back();
    }
    return result;
}
#endif // HELPERS_H