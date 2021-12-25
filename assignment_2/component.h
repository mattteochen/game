/*
 * In component we declare all the components that can be shared with all the entities
 * */

#include "common.h"

namespace component
{
    class CTransform
    {
    public:
        sf::Vector2f    m_pos;
        sf::Vector2f    m_vel;
        float           m_angle = 0;
        CTransform() {}
        CTransform(const sf::Vector2f pos, const sf::Vector2f vel, const float angle);
    };

    class CShape
    {
    public:
        sf::CircleShape     m_shape;
        CShape(const float size, const std::vector<uint8_t> out_colour, const std::vector<uint8_t> inner_colour, const float thickness);
        CShape(const float size, const int sides, const std::vector<uint8_t> out_colour, const std::vector<uint8_t> inner_colour, const float thickness);
    };

    class CCollision
    {
    public:
        float radius = 0;
        CCollision(float rad);
    };

    class CScore
    {
    public:
        int score = 0;
        CScore(int sc);
    };

    class CLifespan
    {
    public:
        int remaining   = 0;
        int total       = 0;
        CLifespan(const int remaining, const int total);
    };

    class CInput
    {
    public:
        bool up     = false;
        bool left   = false;
        bool right  = false;
        bool down   = false;
        bool shoot  = false;
        CInput() {}
    };
}
