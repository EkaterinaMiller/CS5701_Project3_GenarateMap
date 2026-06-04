#ifndef BULLETLIST
#define BULLETLIST
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
class BulletList: public sf::Drawable
{
public:
    class Item: public sf::Drawable
    {
    public:
        Item(const sf::Font& font, const std::string& text, sf::Vector2f position, bool active = false)
            : mBullet(5), mText(font, text, 20), mPosition(position), mActive(active)
        {
            mBullet.setFillColor(sf::Color::White);
            mText.setFillColor(sf::Color::White);
            mBullet.setPosition(mPosition);
            mText.setPosition({mPosition.x + 15, mPosition.y - 10});
        }

        void choose()
        {
            mActive = !mActive;
            mBullet.setFillColor(mActive ? sf::Color::Green : sf::Color::White);
        }
        std::string getText() const { return mText.getString().toAnsiString(); }
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            target.draw(mBullet);
            target.draw(mText);
        }

        bool isActive() const { return mActive; }
    private:
        sf::CircleShape mBullet;
        sf::Text mText;
        sf::Vector2f mPosition;
        bool mActive;
    };
    BulletList(sf::Vector2f position, const sf::Font& font, const std::string& title, const std::vector<std::string>& items)
        : mTitle(font, title, 30), mPosition(position)
    {
        mTitle.setFillColor(sf::Color::White);
        mTitle.setPosition({mPosition.x + 50, mPosition.y + 20});
        for (size_t i = 0; i < items.size(); ++i)
        {
            mItems.emplace_back(font, items[i], sf::Vector2f(mPosition.x + 50, mPosition.y + 70 + i * 40));
        }
    }
    std::vector<std::string> getItems() const;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    sf::Text mTitle;
    std::vector<Item> mItems;
    sf::Vector2f mPosition{0, 0};

};
#endif