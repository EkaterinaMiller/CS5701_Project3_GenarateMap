#include "display.h"
#include "helpers.h"

Display::Display(sf::Font& font)
    : mState(State::Waiting),
      mMapSize({20, 30}, font, "Size", {"50x50", "75x75", "100x100", "200x200"}, true),
      mTerrainType({150, 30}, font, "Terrain", {"Water", "Beach", "Lowland", "Forest", "Hill", "Mountain"}, false),
      mDiagonals({20, 500}, font, "Diagonals", {"Off", "On"}, true),
      mMap({400, 30}, {700, 700}),//position and size of the map display
      mGenarateMap("Generate Map", {150, 450}, {200, 50}, sf::Color::Blue),
      mRun("Run", {150, 670}, {200, 50}, sf::Color::Green),
      mStatus(font, "Status:Waiting...\nChoose Size and Terrain and click Generate Map", 20)
{
    const std::vector<std::vector<int>> demoTiles = {
        {0, 0, 0, 1, 1, 1, 2, 2, 2, 2},
        {0, 0, 1, 1, 1, 2, 2, 3, 3, 2},
        {0, 1, 1, 1, 2, 2, 3, 3, 3, 2},
        {1, 1, 1, 2, 2, 3, 3, 4, 3, 2},
        {1, 1, 2, 2, 3, 3, 4, 4, 3, 2},
        {1, 2, 2, 3, 3, 4, 4, 5, 4, 3},
        {2, 2, 3, 3, 4, 4, 5, 5, 4, 3},
        {2, 3, 3, 4, 4, 5, 5, 5, 4, 3},
        {2, 3, 4, 4, 5, 5, 5, 4, 3, 2},
        {2, 2, 3, 3, 4, 4, 4, 3, 2, 1}
    };
    mMap.setTiles(demoTiles);

    mStatus.setFillColor(sf::Color::White);
    mStatus.setPosition({50, 720});
}
void Display::handleInput(sf::RenderWindow& window)
{
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        mMapSize.handleInput(*event, window);
        mTerrainType.handleInput(*event, window);
        mDiagonals.handleInput(*event, window);
        if (mGenarateMap.handleInput(*event, window)) {
            // Handle generate map action
            std::cout<<"Generate Map button clicked\n";
            std::vector<std::string> mapSizes = mMapSize.getItems();
            for (const auto& size : mapSizes) {
                std::cout << size << "\n";
            }
            std::vector<std::string> terrainTypes = mTerrainType.getItems();
            for (const auto& terrain : terrainTypes) {
                std::cout << terrain << "\n";
            }
            mStatus.setString("Status: Generating Map...\nSize: " + vectorToString(mapSizes) 
                + "\nTerrain: " + vectorToString(terrainTypes));

        }
        mDiagonals.update();
        if (mRun.handleInput(*event, window)) {
            // Handle run action
            std::cout<<"Run button clicked\n";
            mStatus.setString("Status: Running...\nWith Diagonals: " + 
                vectorToString(mDiagonals.getItems()));
        }
        mRun.handleInput(*event, window);
        // Handle input for bullet lists if needed
    }
}
void Display::update(sf::RenderWindow& window)
{
    // Implementation for updating the display
    mMap.update();
    mMapSize.update();
    mTerrainType.update();
    mGenarateMap.update();
    mRun.update();
    mDiagonals.update();
}
void Display::render(sf::RenderWindow& window)
{
    window.clear();
    window.draw(mMapSize);
    window.draw(mTerrainType);
    window.draw(mDiagonals);
    window.draw(mMap);  
    window.draw(mGenarateMap);
    window.draw(mRun);
    window.draw(mStatus);
    window.display();
}