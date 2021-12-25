#include "entity.h"

int main (int agrc, char *argv[])
{
    /* create new entity */
    entity::Manager entity_manager;
    auto e = entity_manager.addEntity("default");

    /* create a new component for this entity */
    e->p_CTransform = std::make_shared<component::CTransform>(sf::Vector2f(100.0f,100.0f), sf::Vector2f(0.1f,0.1f));
    e->p_CShape = std::make_shared<component::CShape>((float)50, sf::Vector2f(100.0f,100.0f), (float)0.1, std::vector<uint8_t> {100,100,100});
    
    std::cout << e->p_CTransform->m_pos.x << " " << e->p_CTransform->m_pos.y << "\n";

    return 0;
}
