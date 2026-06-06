#include "display.h"
#include "helpers.h"
#include <cstdlib>
#include <time.h>

Display::Display(sf::Font& font)
    : mState(State::Waiting),
      mMapSize({10, 30}, font, "Size", {"50x50", "75x75", "100x100", "200x200"}, true),
      mTerrainType({180, 30}, font, "Terrain", {"Water", "Beach", "Lowland", "Forest", "Hill","Ridges", "Mountain", "Snow Tops"}, false),
      mDiagonals({10, 500}, font, "Diagonals", {"Off", "On"}, true),
      mAnimation({180, 500}, font, "Animation", {"Off", "On"}, true),
      mMap({450, 30}, {700, 700}, mCurrentMap),//position and size of the map display
      mGenarateMap("Generate Map", {150, 450}, {200, 50}, sf::Color::Blue),
      mRun("Run", {150, 670}, {200, 50}, sf::Color::Green),
      mStatus(font, "Status:Waiting...\nChoose Size and Terrain and click Generate Map", 20)
{
    mCurrentMap = {{0}};//User have not choose any options, so we display a simple map with one tile. 
    mMap.rebuildTiles();
    mStatus.setFillColor(sf::Color::White);
    mStatus.setPosition({50, 720});
    mMapSize.selectItems(0);//Default size is 50x50
    mTerrainType.selectItems(0);//Default terrain type is Water
    mTerrainType.selectItems(1);//Default terrain type is Beach
    mTerrainType.selectItems(2);//Default terrain type is Lowland
    mTerrainType.selectItems(3);//Default terrain type is Forest
    mTerrainType.selectItems(4);//Default terrain type is Hill
    mTerrainType.selectItems(6);//Default terrain type is Mountain
    mDiagonals.selectItems(0);//Default diagonals option is Off
    mAnimation.selectItems(1);//Default animation option is On
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
            mAnimation.handleInput(*event, window);
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
                    mMap.rebuildTiles();
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
                        }else if (item == "Ridges") {
                            palette.push_back(TERRAIN_COLORS[TerrainType::RIDGES]);
                        }else if (item == "Mountain") {
                            palette.push_back(TERRAIN_COLORS[TerrainType::MOUNTAIN]);
                        }else if (item == "Snow Tops") {
                            palette.push_back(TERRAIN_COLORS[TerrainType::SNOWTOPS]);
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
                }else if (mAnimation.getItems().empty()) {
                    mStatus.setString("Status: Please select animation option before running the algorithm.");
                }else {
                    mStatus.setString("Status: Running...\nWith Animation: " + 
                        vectorToString(mAnimation.getItems()));
                    mStartTime.restart();
                    mTerrainType.grayOutList();
                    mMapSize.grayOutList();
                    mDiagonals.grayOutList();
                    mState = State::Running;
                }
            }
        }else if (mState == State::Running) {
            mAnimation.handleInput(*event, window);
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
        mAnimation.update();//bullet list have their own update function to handle the color change
        mGenarateMap.update();//button have their own update function to handle the color change when clicked
        mRun.update();//button have their own update function to handle the color change when clicked
        mMap.update();
        minConflicts = std::numeric_limits<int>::max();
        numStuckIterations = 0;
        numOfIterations = 0;
    }else if (mState == State::Running) {
        mMapSize.update();
        mTerrainType.update();
        mDiagonals.update();
        mAnimation.update();
        mRun.update();
        std::vector<std::string> terrainTypes = mTerrainType.getItems();
        std::vector<std::string> diagonalOptions = mDiagonals.getItems();
        int minValue = 1;
        int maxValue = terrainTypes.size();

        const bool animationOn = mAnimation.getItems()[0] == "On";
        const int passesPerFrame = animationOn ? 1 : 200;
        int lastNumConflicts = minConflicts;

        for (int pass = 0; pass < passesPerFrame && mState == State::Running; ++pass) {
            int numConflicts = resolveConflictOnePass(mCurrentMap, minValue, maxValue, diagonalOptions[0] == "On");
            lastNumConflicts = numConflicts;
            numOfIterations++;

            if (numConflicts < minConflicts) {
                minConflicts = numConflicts;
                numStuckIterations = 0;

                if (animationOn) {
                    mMap.recolorTiles();
                    mStatus.setString("Status: Running...\nTotal Conflicts: " + std::to_string(numConflicts)
                        + "\nIterations: " + std::to_string(numOfIterations) );
                }

                if (numConflicts == 0) {
                    mMap.recolorTiles();
                    sf::Time elapsed = mStartTime.getElapsedTime();
                    mStatus.setString("Status: Finished!\nTotal Conflicts: " + std::to_string(numConflicts)
                        + "\nElapsed Time: " + std::to_string(elapsed.asSeconds()) + " seconds Iterations: " + std::to_string(numOfIterations));
                    mState = State::Waiting;
                    mTerrainType.makeActive();
                    mMapSize.makeActive();
                    mDiagonals.makeActive();
                }
            }else {
                numStuckIterations++;
                if (numStuckIterations > 300) {
                    mMap.recolorTiles();
                    mStatus.setString("Status: Stuck at " + std::to_string(numConflicts) + " conflicts\nYou may genarate new map.");
                    mState = State::Waiting;
                    mTerrainType.makeActive();
                    mMapSize.makeActive();
                    mDiagonals.makeActive();
                }
            }
        }

        if (!animationOn && mState == State::Running) {
            mStatus.setString("Status: Running...\nTotal Conflicts: " + std::to_string(lastNumConflicts)
                + "\nIterations: " + std::to_string(numOfIterations));
        }
    }
}
void Display::render(sf::RenderWindow& window)
{
    window.clear();
    window.draw(mMapSize);
    window.draw(mTerrainType);
    window.draw(mDiagonals);
    window.draw(mAnimation);
    window.draw(mMap);  
    window.draw(mGenarateMap);
    window.draw(mRun);
    window.draw(mStatus);
    window.display();
}