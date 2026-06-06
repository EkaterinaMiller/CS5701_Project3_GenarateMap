#ifndef OPTIONS_H
#define OPTIONS_H 
#include <SFML/Graphics.hpp>  
const sf::Color TERRAIN_COLORS[] = {
    sf::Color::White,               // 0
    sf::Color::Blue,                 // 1
    sf::Color::Yellow,               // 2
    sf::Color(47, 255, 6),          // 3
    sf::Color(7, 159, 17),          // 4
    sf::Color(102, 102, 0),        // 5
    sf::Color(99, 99, 68),         // 6
    sf::Color(128, 128, 128),         // 7
    sf::Color(250, 250, 250)         // 8
};
enum TerrainType {NOTHING, WATER, BEACH, LOWLAND, FOREST, HILL,RIDGES, MOUNTAIN, SNOWTOPS};
#endif