/*
 * In component we declare all the components that can be shared with all the entities
 * */

#include "../common/create_figure.h"

namespace component
{
    class CTransform
    {
    public:
        sf::Vector2f    m_pos;
        sf::Vector2f    m_vel;
        CTransform() {}
        CTransform(const sf::Vector2f pos, const sf::Vector2f vel) : m_pos(pos), m_vel(vel) {}
    };

    class CShape
    {
    public:
        create_figure::circleSfmlObject     m_shape;
        CShape(const float size, const sf::Vector2f pos, const float speed, const std::vector<uint8_t> colour) : m_shape(size, pos, speed, colour) {}
        CShape(const float size, const int sides, const sf::Vector2f pos, const float speed, const std::vector<uint8_t> colour) : m_shape(size, sides, pos, speed, colour) {}
    };

}
