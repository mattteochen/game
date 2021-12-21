#ifndef CREATE_FIGURE_H
#define CREATE_FIGURE_H

#include "config.h"

namespace create_figure
{
    class sfmlObjectCommon
    {
    protected:
        float speed;
        sf::Vector2f position;
        std::vector<uint8_t> colour;

    public:
        sfmlObjectCommon(const float speed, const sf::Vector2f pos, const std::vector<uint8_t> colour);
        /* TODO, add references return type */
        const float                 getMoveSpeed();
        const sf::Vector2f          getPosition();
        const std::vector<uint8_t>  getRGB();
        void                        setMoveSpeed(const float speed);
        void                        setRGBInternal(const std::vector<uint8_t> colour);
        void                        setPositionInternal(const sf::Vector2f pos);
    };

    class circleSfmlObject: public sfmlObjectCommon
    {
    private:
    public:
        sf::CircleShape             circleFigure;
        circleSfmlObject(const float size, const float num_sides, const sf::Vector2f pos, const float speed, const std::vector<uint8_t> colour);
        circleSfmlObject(const float size,  const sf::Vector2f pos, const float speed, const std::vector<uint8_t> colour);
        void                        setPosition(const sf::Vector2f pos);
        void                        setRGB(const std::vector<uint8_t> colour);
    };

}

#endif
