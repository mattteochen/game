#ifndef CREATE_FIGURE_H
#define CREATE_FIGURE_H

#include "config.h"

namespace create_figure
{
    class sfmlObjectCommon
    {
    protected:
        float                   speed;
        sf::Vector2f            position;
        std::vector<uint8_t>    colour;
        std::pair<int,int>      direction;

    public:
        sfmlObjectCommon(const float speed, const sf::Vector2f pos, const std::vector<uint8_t> colour);
        /* TODO, add references return type */
        const float                 getMoveSpeed();
        const sf::Vector2f          getPosition();
        const std::vector<uint8_t>  getRGB();
        const std::pair<int,int>    getDirection();
        void                        setMoveSpeed(const float speed);
        void                        setRGBInternal(const std::vector<uint8_t> colour);
        void                        setDirection(const std::pair<int,int> &new_direction);
        void                        setPositionInternal(const sf::Vector2f pos);
        /* TODO: attach the text ? */
    };

    class circleSfmlObject: public sfmlObjectCommon
    {
    private:
    public:
        sf::CircleShape             circleFigure;
        sf::Text                    circleFigureText;
        circleSfmlObject(const float size, const sf::Vector2f pos, const float speed, const std::vector<uint8_t> colour);
        circleSfmlObject(const float size, const int sides, const sf::Vector2f pos, const float speed, const std::vector<uint8_t> colour);
        void                        setPosition(const sf::Vector2f pos);
        void                        setRGB(const std::vector<uint8_t> colour);
        void                        setText(const std::string text, sf::Font &font, const unsigned int size);
    };

    /* calculate the font pos bases on the object pos */
    auto calculate_centered_font_in_circle = [](const sf::Vector2f &object_pos, sf::Vector2f &font_pos, const float object_size, const float font_width, const float font_heigth)
    {
        float diff_y = abs(object_size - font_heigth); /* do not div 2 for circles */
        float diff_x = abs(object_size - font_width);

        font_pos.y = object_pos.y + diff_y;
        font_pos.x = object_pos.x + object_size - (font_width / 2);
    };
}

#endif
