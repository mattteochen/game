#include "component.h"

component::CShape::CShape(const float size, const std::vector<uint8_t> out_colour, const std::vector<uint8_t> inner_colour, const float thickness) : m_shape(size)
{
    m_shape.setFillColor(sf::Color(inner_colour[0],inner_colour[1],inner_colour[2]));
    m_shape.setOutlineThickness(thickness);
    m_shape.setOutlineColor(sf::Color(out_colour[0],out_colour[1],out_colour[2]));
    m_shape.setOrigin(size,size);
}

component::CShape::CShape(const float size, const int sides, const std::vector<uint8_t> out_colour, const std::vector<uint8_t> inner_colour, const float thickness) : m_shape(size, sides)
{
    m_shape.setFillColor(sf::Color(inner_colour[0],inner_colour[1],inner_colour[2]));
    m_shape.setOutlineThickness(thickness);
    m_shape.setOutlineColor(sf::Color(out_colour[0],out_colour[1],out_colour[2]));
    m_shape.setOrigin(size,size);
}

component::CTransform::CTransform(const sf::Vector2f pos, const sf::Vector2f vel, const float angle) : m_pos(pos), m_vel(vel), m_angle(angle) {}

component::CCollision::CCollision(float rad) : radius(rad) {}

component::CScore::CScore(int sc) : score(sc) {}

component::CLifespan::CLifespan(const int remaining, const int total) : remaining(remaining), total(total) {}
