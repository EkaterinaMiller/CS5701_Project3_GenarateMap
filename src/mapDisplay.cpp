#include "mapDisplay.h"
#include <algorithm>

MapDisplay::MapDisplay(sf::Vector2f position, sf::Vector2f size, const std::vector<std::vector<int>>& tileValues)
    : mPosition(position),
      mSize(size),
      mTileValues(tileValues),
      mPalette({ sf::Color::White})
{
    //mStartBoard.setPosition(mPosition);
    //mStartBoard.setSize(mSize);
    //mStartBoard.setFillColor(sf::Color::Black);
    //mStartBoard.setOutlineThickness(1.f);
    //mStartBoard.setOutlineColor(sf::Color(90, 90, 90));
}

void MapDisplay::setTiles()
{
    rebuildTiles();
}

void MapDisplay::setPalette(const std::vector<sf::Color>& palette)
{
    if (!palette.empty())
    {
        mPalette = palette;
        rebuildTiles();
    }
}

sf::Color MapDisplay::colorForValue(int value) const
{
    if (value < 0 || mPalette.empty())
    {
        return sf::Color::Magenta;
    }

    const std::size_t index = static_cast<std::size_t>(value);
    if (index < mPalette.size())
    {
        return mPalette[index];
    }

    return sf::Color::Magenta;
}

void MapDisplay::rebuildTiles()
{
    mTiles.clear();
    if (mTileValues.empty() || mTileValues.front().empty())
    {
        throw std::runtime_error("Tile values cannot be empty");
    }

    const std::size_t rows = mTileValues.size();
    std::size_t cols = 0;
    //Sanety check. We expect col = row, but we want to be sure
    for (const auto& row : mTileValues)
    {
        cols = std::max(cols, row.size());
    }
    if (cols == 0)
    {
        throw std::runtime_error("Invalid tile dimensions");
    }

    const float tileWidth = mSize.x / static_cast<float>(cols);
    const float tileHeight = mSize.y / static_cast<float>(rows);

    for (std::size_t r = 0; r < rows; ++r)
    {
        for (std::size_t c = 0; c < mTileValues[r].size(); ++c)
        {
            sf::RectangleShape tile({tileWidth, tileHeight});
            tile.setPosition({
                mPosition.x + static_cast<float>(c) * tileWidth,
                mPosition.y + static_cast<float>(r) * tileHeight
            });
            tile.setFillColor(colorForValue(mTileValues[r][c]));
            tile.setOutlineThickness(0.5f);
            tile.setOutlineColor(sf::Color(25, 25, 25));
            mTiles.push_back(tile);
        }
    }
}
void MapDisplay::recolorTiles()
{
    for (std::size_t i = 0; i < mTiles.size(); ++i)
    {
        const std::size_t row = i / mTileValues.front().size();
        const std::size_t col = i % mTileValues.front().size();
        mTiles[i].setFillColor(colorForValue(mTileValues[row][col]));
    }
}

void MapDisplay::update()
{
    // Update logic for the map display can be added here if needed
    recolorTiles();
}

void MapDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //target.draw(mStartBoard, states);
    for (const auto& tile : mTiles)
    {
        target.draw(tile, states);
    }
}

