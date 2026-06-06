#include "display.h"
#include "helpers.h"
#include <cstdlib>
#include <time.h>

Display::Display(sf::Font& font)
    : mState(State::Waiting),
      mMapSize({20, 30}, font, "Size", {"50x50", "75x75", "100x100", "200x200"}, true),
      mTerrainType({150, 30}, font, "Terrain", {"Water", "Beach", "Lowland", "Forest", "Hill", "Mountain"}, false),
      mDiagonals({20, 500}, font, "Diagonals", {"Off", "On"}, true),
      mMap({400, 30}, {700, 700}, mCurrentMap),//position and size of the map display
      mGenarateMap("Generate Map", {150, 450}, {200, 50}, sf::Color::Blue),
      mRun("Run", {150, 670}, {200, 50}, sf::Color::Green),
      mStatus(font, "Status:Waiting...\nChoose Size and Terrain and click Generate Map", 20)
{
    mCurrentMap = {{0}};//User have not choose any options, so we display a simple map with one tile. 
    mMap.setTiles();

    mStatus.setFillColor(sf::Color::White);
    mStatus.setPosition({50, 720});
    srand(static_cast<unsigned int>(time(nullptr)));
}
void Display::handleInput(sf::RenderWindow& window)
{
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        if (mState == State::Waiting) {
            mMapSize.handleInput(*event, window);
            mTerrainType.handleInput(*event, window);
            mDiagonals.handleInput(*event, window);
            if (mGenarateMap.handleInput(*event, window)) {
                // Handle generate map action
                std::vector<std::string> mapSizes = mMapSize.getItems();
                std::vector<std::string> terrainTypes = mTerrainType.getItems();
                if (mapSizes.empty() || terrainTypes.empty()) {
                    mStatus.setString("Status: Please select both size and terrain type before generating the map.");
                    return;
                }else {
                    mStatus.setString("Status: Generating Map...\nSize: " + vectorToString(mapSizes) 
                    + "\nTerrain: " + vectorToString(terrainTypes));
                
                    // Generate the map using the min-conflicts algorithm
                    int minValue = 1;
                    int maxValue = terrainTypes.size();
                    int size = std::stoi(mapSizes[0]);
                    mCurrentMap = generateMap(minValue, maxValue, size);
                    mMap.setTiles();
                    std::vector<sf::Color> palette{ sf::Color::White}; // Default color for 0  
                    for(const auto& item: terrainTypes){
                        if (item == "Water") {
                            palette.push_back(TERRAIN_COLORS[TerrainType::WATER]);
                        }else if (item == "Beach") {
                            palette.push_back(TERRAIN_COLORS[TerrainType::BEACH]);
                        }else if (item == "Lowland") {
                            palette.push_back(TERRAIN_COLORS[TerrainType::LOWLAND]);
                        }else if (item == "Forest") {
                            palette.push_back(TERRAIN_COLORS[TerrainType::FOREST]);
                        }else if (item == "Hill") {
                            palette.push_back(TERRAIN_COLORS[TerrainType::HILL]);
                        }else if (item == "Mountain") {
                            palette.push_back(TERRAIN_COLORS[TerrainType::MOUNTAIN]);
                        }
                    }
                    mMap.setPalette(palette);
                    mStatus.setString("Status: Map Generated!\nClick Run to start the algorithm.");
                }
            }
            if (mRun.handleInput(*event, window)) {
                if (mDiagonals.getItems().empty()){
                    mStatus.setString("Status: Please select diagonal option before running the algorithm.");
                }else if (mCurrentMap.empty() || mCurrentMap.front().empty()) {
                    mStatus.setString("Status: Please generate a map before running the algorithm.");
                }else {
                    mStatus.setString("Status: Running...\nWith Diagonals: " + 
                        vectorToString(mDiagonals.getItems()));
                    mStartTime.restart();
                    mState = State::Running;
                }
            }
        }else if (mState == State::Running) {

        }
    }
}
void Display::update(sf::RenderWindow& window)
{
    static int minConflicts = std::numeric_limits<int>::max();
    static int numStuckIterations = 0;
    static int numOfIterations = 0;
    // Implementation for updating the display
    if (mState == State::Waiting) {
        mMapSize.update();//bullet list have their own update function to handle the color change when clicked
        mTerrainType.update();//bullet list have their own update function to handle the color change when clicked
        mDiagonals.update();//bullet list have their own update function to handle the color change
        mGenarateMap.update();//button have their own update function to handle the color change when clicked
        mRun.update();//button have their own update function to handle the color change when clicked
        mMap.update();
        minConflicts = std::numeric_limits<int>::max();
        numStuckIterations = 0;
        numOfIterations = 0;
    }else if (mState == State::Running) {
        std::vector<std::string> terrainTypes = mTerrainType.getItems();
        std::vector<std::string> diagonalOptions = mDiagonals.getItems();
        int minValue = 1;
        int maxValue = terrainTypes.size();

        int numConflicts = resolveConflictOnePass(mCurrentMap, minValue, maxValue, diagonalOptions[0] == "On");
        numOfIterations++;
        if (numConflicts < minConflicts) {
            minConflicts = numConflicts;
            numStuckIterations = 0;
            // Rebuild from current map data so visual updates stay in sync with algorithm steps.
            mMap.setTiles();
            //numConflicts = countTotalConflicts(mCurrentMap, diagonalOptions[0] == "On");
            mStatus.setString("Status: Running...\nTotal Conflicts: " + std::to_string(numConflicts)
                + "\nIterations: " + std::to_string(numOfIterations) );
    
            // Stop only when solved; otherwise keep running more passes.
            if (numConflicts == 0) {
                sf::Time elapsed = mStartTime.getElapsedTime();
                mStatus.setString("Status: Finished!\nTotal Conflicts: " + std::to_string(numConflicts) 
                    + "\nElapsed Time: " + std::to_string(elapsed.asSeconds()) + " seconds Iterations: " + std::to_string(numOfIterations));
                mState = State::Waiting;
            }
        }else {
            numStuckIterations++;
            if (numStuckIterations > 300) {
                mStatus.setString("Status: Stuck at " + std::to_string(numConflicts) + " conflicts\nYou may genarate new map.");
                mState = State::Waiting;
            }
        }
    }
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