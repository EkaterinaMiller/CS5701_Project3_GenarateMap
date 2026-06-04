#include "bulletList.h"

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
        if (item.isActive())
        {
            items.push_back(item.getText());
        }
    }
    return items;
}