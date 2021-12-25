/*
 * In entity we declare all entity class 
 * */

#include "../common/create_figure.h"
#include "component.h"
#include <memory>

namespace entity
{
    class Entity
    {
    private:
        const size_t        m_id = 0;
        const std::string   m_tag;
        bool                m_alive = true;
        
        /* make the constructor private so every add process is handled by the Manager class */
        Entity() {}
        Entity(const std::string tag, const size_t id) : m_id(id), m_tag(tag) {}
        friend class Manager;
    public:
        std::shared_ptr<component::CTransform>      p_CTransform;
        std::shared_ptr<component::CShape>          p_CShape;
        
        bool                isAlive();
        const size_t        getId();
        const std::string   getTag();
        void                destroy();

    };

    typedef std::vector<std::shared_ptr<Entity>> eVec;
    typedef std::map<std::string, eVec> eMap;

    class Manager
    {
        eVec    m_entities;
        eVec    m_to_add; /* we do wanna change the vector while iterating on it */
        eMap    m_entities_map;
        size_t  m_num = 0;
    public:
        Manager(){}

        /* create new entity */
        std::shared_ptr<Entity> addEntity(const std::string tag);
        /* get the game entities */
        eVec    &getEntities();
        eVec    &getEntitiesTag(const std::string tag);
        /* update the vector every time frame */
        void    update();
    };

}
