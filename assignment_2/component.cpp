#include "component.h"

component::CShape::CShape(const float size, const std::vector<uint8_t> inner_color, const std::vector<uint8_t> out_color, const float thickness) : m_shape(size)
{
    m_shape.setFillColor(sf::Color(inner_color[0],inner_color[1],inner_color[2]));
    m_shape.setOutlineThickness(thickness);
    m_shape.setOutlineColor(sf::Color(out_color[0],out_color[1],out_color[2]));
    m_shape.setOrigin(size,size);
}

component::CShape::CShape(const float size, const int sides, const std::vector<uint8_t> inner_color, const std::vector<uint8_t> out_color, const float thickness) : m_shape(size, sides)
{
    m_shape.setFillColor(sf::Color(inner_color[0],inner_color[1],inner_color[2]));
    m_shape.setOutlineThickness(thickness);
    m_shape.setOutlineColor(sf::Color(out_color[0],out_color[1],out_color[2]));
    m_shape.setOrigin(size,size);
}

component::CShape::CShape(const float size, const sf::Color inner_color, const sf::Color out_color, const float thickness) : m_shape(size)
{
    m_shape.setFillColor(sf::Color(inner_color));
    m_shape.setOutlineThickness(thickness);
    m_shape.setOutlineColor(out_color);
    m_shape.setOrigin(size,size);
}

component::CShape::CShape(const float size, const int sides, const sf::Color inner_color, const sf::Color out_color, const float thickness) : m_shape(size, sides)
{
    m_shape.setFillColor(sf::Color(inner_color));
    m_shape.setOutlineThickness(thickness);
    m_shape.setOutlineColor(out_color);
    m_shape.setOrigin(size,size);
}

component::CTransform::CTransform(const sf::Vector2f pos, const sf::Vector2f vel, const float angle) : m_pos(pos), m_vel(vel), m_angle(angle) {}

component::CCollision::CCollision(float rad) : radius(rad) {}

component::CScore::CScore(int sc) : score(sc) {}

component::CLifespan::CLifespan(const int remaining, const int total) : remaining(remaining), total(total) {}
