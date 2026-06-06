#include "bulletList.h"
sf::FloatRect BulletList::Item::getGlobalBounds() const { 
            float positionX = std::min(mBullet.getGlobalBounds().position.x, mText.getGlobalBounds().position.x);
            float positionY = std::min(mBullet.getGlobalBounds().position.y, mText.getGlobalBounds().position.y);
            float sizeX = 15.0f + mText.getGlobalBounds().size.x;
            float sizeY = 20.0f;
            return sf::FloatRect({positionX, positionY}, {sizeX, sizeY}); 
        }
void BulletList::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(mTitle);
        for (const auto& item : mItems)
            target.draw(item);
    }

std::vector<std::string> BulletList::getItems() const
{
    std::vector<std::string> items;
    for (const auto& item : mItems)
    {
        if (item.getStatus() == status::clicked)
        {
            items.push_back(item.getText());
        }
    }
    return items;
}
void BulletList::handleInput(const sf::Event& e, sf::RenderWindow& window)
{
    const sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    const sf::Vector2f mousePosition = window.mapPixelToCoords(mousePixelPos);
    for (auto& item : mItems)
    {
        const bool mouseInItem = item.getGlobalBounds().contains(mousePosition);
        if (const auto* mousePressed = e.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mousePressed->button == sf::Mouse::Button::Left)
            {
                if (mouseInItem){
                    item.setStatus(item.getStatus() == status::clicked ? status::normal : status::clicked );
                    if (mOnlyOne && item.getStatus() == status::clicked) {
                        for (auto& otherItem : mItems) {
                            if (&otherItem != &item) {
                                otherItem.setStatus(status::normal);
                            }
                        }
                    }
                }
            }
        }
    }
}

void BulletList::update()
{
    for (auto& item : mItems)
    {
        switch (item.getStatus())
        {
        case status::normal:
            item.setColor(INACTIVE_COLOR, INACTIVE_COLOR);
            break;
        case status::clicked:
            item.setColor(ACTIVE_COLOR, INACTIVE_COLOR);
            break;
        }
    }
}
void BulletList::selectItems(int index)
{
    if (index < 0 || index >= static_cast<int>(mItems.size())) {
        throw std::out_of_range("Index out of range");
    }

    mItems[index].setStatus(status::clicked);

}