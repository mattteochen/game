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
        sfmlObjectCommon(float &speed, sf::Vector2f &pos, std::vector<uint8_t> &colour);
        /* TODO, add references return type */
        const float                 getMoveSpeed();
        const sf::Vector2f          getPosition();
        const std::vector<uint8_t>  getRGB();
        void                        setMoveSpeed(const float &speed);
        void                        setRGBInternal(std::vector<uint8_t> &colour);
        void                        setPositionInternal(const sf::Vector2f &pos);
    };

    class circleSfmlObject: public sfmlObjectCommon
    {
    private:
    public:
        sf::CircleShape             circleFigure;
        circleSfmlObject(float size, float num_sides, sf::Vector2f &pos, float speed, std::vector<uint8_t> &colour);
        circleSfmlObject(float size,  sf::Vector2f &pos, float speed, std::vector<uint8_t> &colour);
        void                        setPosition(const sf::Vector2f &pos);
        void                        setRGB(std::vector<uint8_t> &colour);
    };

}

#endif
