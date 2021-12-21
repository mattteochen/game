/* custom SFML figure creation API */
#include "create_figure.h"
#include "SFML/Graphics/Color.hpp"

/* create the common sfml object configuration */
create_figure::sfmlObjectCommon::sfmlObjectCommon(const float speed, const sf::Vector2f pos, const std::vector<uint8_t> colour) : speed(speed), position(pos), colour(colour) {}

/* create new sfml figure */
create_figure::circleSfmlObject::circleSfmlObject(const float size, const float num_sides, const sf::Vector2f pos, const float speed, const std::vector<uint8_t> colour) : circleFigure(size, num_sides), sfmlObjectCommon(speed, pos, colour)
{
    this -> circleFigure.setPosition(pos.x, pos.y);
}

create_figure::circleSfmlObject::circleSfmlObject(const float size,  const sf::Vector2f pos, const float speed, const std::vector<uint8_t> colour) : circleFigure(size), sfmlObjectCommon(speed, pos, colour)
{
    this -> circleFigure.setPosition(pos.x, pos.y);
}

/* set figure new position */
void
create_figure::circleSfmlObject::setPosition(const sf::Vector2f pos)
{
    this -> circleFigure.setPosition(pos.x, pos.y);
    setPositionInternal(pos);
}

/* set figure new colour */
void
create_figure::circleSfmlObject::setRGB(const std::vector<uint8_t> colour)
{
    if (colour.size() != RGB_COLOUR_LEN)
    {
        std::cerr << "RGB setting is wrong!\n";
        return;
    }
    this -> circleFigure.setFillColor(sf::Color(colour[0], colour[1], colour[2]));
    setRGBInternal(colour);
}

/* set move speed */
void 
create_figure::sfmlObjectCommon::setMoveSpeed(const float speed)
{
    this -> speed = speed;
}

/* update the internal class position */
void
create_figure::sfmlObjectCommon::setPositionInternal(const sf::Vector2f pos)
{
    this -> position = pos;
}

/* update internal RGB */
void 
create_figure::sfmlObjectCommon::setRGBInternal(const std::vector<uint8_t> colour)
{
    this -> colour = colour;
}


const float
create_figure::sfmlObjectCommon::getMoveSpeed()
{
    return this -> speed;
}

const sf::Vector2f              
create_figure::sfmlObjectCommon::getPosition()
{
    return this -> position;
}

const std::vector<uint8_t>  
create_figure::sfmlObjectCommon::getRGB()
{
    return this -> colour;
}
