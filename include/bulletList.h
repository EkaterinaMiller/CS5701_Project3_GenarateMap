#ifndef BULLETLIST
#define BULLETLIST
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
const sf::Color ACTIVE_COLOR = sf::Color::Red;
const sf::Color INACTIVE_COLOR = sf::Color::White;
const sf::Color HOVER_COLOR = sf::Color::Yellow;
class BulletList: public sf::Drawable
{
    public:
    enum status {normal, clicked};
    class Item: public sf::Drawable
    {
    public:
        Item(const sf::Font& font, const std::string& text, sf::Vector2f position, bool active = false)
            : mBullet(5), mText(font, text, 20), mPosition(position), mStatus(status::normal)
        {
            mBullet.setFillColor(INACTIVE_COLOR);
            mText.setFillColor(sf::Color::White);
            mBullet.setPosition(mPosition);
            mText.setPosition({mPosition.x + 15, mPosition.y - 10});
        }
        std::string getText() const { return mText.getString().toAnsiString(); }
        void setColor(sf::Color mBulletColor, sf::Color mTextColor)
        {
            mBullet.setFillColor(mBulletColor);
            mText.setFillColor(mTextColor);
        }
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            target.draw(mBullet);
            target.draw(mText);
        }
        sf::FloatRect getGlobalBounds() const;
        void setStatus(status newStatus) { mStatus = newStatus; }
        status getStatus() const { return mStatus; }
    private:
        sf::CircleShape mBullet;
        sf::Text mText;
        sf::Vector2f mPosition;
        status mStatus;
    };
    BulletList(sf::Vector2f position, const sf::Font& font, const std::string& title, const std::vector<std::string>& items, bool onlyOne = true, bool greyOut = false)
        : mTitle(font, title, 30), mPosition(position), mOnlyOne(onlyOne), mGreyOut(greyOut)
    {
        mTitle.setFillColor(sf::Color::White);
        mTitle.setPosition({mPosition.x + 50, mPosition.y + 20});
        for (size_t i = 0; i < items.size(); ++i)
        {
            mItems.emplace_back(font, items[i], sf::Vector2f(mPosition.x + 50, mPosition.y + 70 + i * 40));
        }
    }
    std::vector<std::string> getItems() const;
    void selectItems(int index);
    void handleInput(const sf::Event& e, sf::RenderWindow& window);
    void update();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    sf::Text mTitle;
    std::vector<Item> mItems;
    sf::Vector2f mPosition{0, 0};
    bool mOnlyOne{true}; // If true, only one item can be active at a time
    bool mGreyOut{false}; // If true, inactive items are greyed out

};
#endif