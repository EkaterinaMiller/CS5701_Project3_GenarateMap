#ifndef DISPLAY_H
#define DISPLAY_H
#include "bulletList.h"
#include "mapDisplay.h"
#include "button.h"
enum class State
{
    Waiting,
    Running
};
#include <SFML/Graphics.hpp>    
class Display
{
public:
    Display(sf::Font& font);
    ~Display(){};

    void handleInput(sf::RenderWindow& mWindow);
    void update(sf::RenderWindow& mWindow);
    void render(sf::RenderWindow& mWindow);
    //bool isDone() const;

private:
    //sf::RenderWindow mWindow;
    // bool mIsDone;
    State mState;
    BulletList mTerrainType;
    BulletList mMapSize;
    BulletList mDiagonals;
    MapDisplay mMap;
    Button mGenarateMap;
    Button mRun;
    sf::Text mStatus;

};
#endif