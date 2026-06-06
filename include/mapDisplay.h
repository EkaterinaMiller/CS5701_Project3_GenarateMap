#ifndef MAPDISPLAY_H
#define MAPDISPLAY_H 
#include <SFML/Graphics.hpp>
#include <vector>
#include "options.h"

class MapDisplay:public sf::Drawable
{ 
public:
    MapDisplay(sf::Vector2f position, sf::Vector2f size, const std::vector<std::vector<int>>& tileValues);
    //void setTiles(); same as rebuild.
    void setPalette(const std::vector<sf::Color>& palette);
    void rebuildTiles();
    void recolorTiles();
    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    ~MapDisplay(){};

private:
    sf::Color colorForValue(int value) const;

    //sf::RectangleShape mStartBoard;
    sf::Vector2f mPosition;
    sf::Vector2f mSize;
    const std::vector<std::vector<int>>& mTileValues;
    std::vector<sf::Color> mPalette;
    std::vector<sf::RectangleShape> mTiles;
};
#endif