#include "entity.h"

bool
entity::Entity::isAlive()
{
    return this->m_alive;
}

const size_t        
entity::Entity::getId()
{
    return this->m_id;
}
    
const std::string
entity::Entity::getTag()
{
    return this->m_tag;
}

void
entity::Entity::destroy()
{
    this->m_alive = 0;
}

std::shared_ptr<entity::Entity>
entity::Manager::addEntity(const std::string tag)
{
    auto e = std::shared_ptr<entity::Entity>(new entity::Entity (tag, m_num++));
    m_to_add.push_back(e);
    return e;
}

entity::eVec&
entity::Manager::getEntitiesTag(const std::string tag)
{
    return m_entities_map[tag];
}

entity::eVec&
entity::Manager::getEntities()
{
    return m_entities;
}

void
entity::Manager::update()
{
    /* helper lambda to remove entities from map */
    auto erase_from_map = [&](const std::string &tag, const size_t &id) -> void
    {
        for (auto it = m_entities_map[tag].begin(); it != m_entities_map[tag].end(); it++)
        {
            if ((*it)->getId() == id)
            {
                it = m_entities_map[tag].erase(it);
                return;
            }
        }
    };

    /* add the new entities */
    for (auto &e : m_to_add)
    {
        /* update the whole vector */
        m_entities.push_back(e);
        /* update the map */
        m_entities_map[e->getTag()].push_back(e);
    }
    m_to_add.clear();


    /* remove the death entities */
    for (auto it = m_entities.begin(); it != m_entities.end();)
    {
        if (!(*it)->isAlive()) 
        {
            if (!(*it)->isAlive())
            {
                /* store data to erase for map erasing */
                std::string tag = (*it)->getTag();
                size_t id = (*it)->getId();
                /* erase form vector */
                it = m_entities.erase(it);
                /* erase from map */
                erase_from_map(tag, id);
            }
        }
        else it++;
    }        
}

