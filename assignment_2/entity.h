/*
 * In entity we declare all entity class 
 * */

#include "component.h"

namespace entity
{
    class Entity
    {
        friend class Manager;
    private:
        const size_t        m_id = 0;
        const std::string   m_tag;
        bool                m_alive = true;
        
        /* make the constructor private so every add process is handled by the Manager class */
        Entity() {}
        Entity(const std::string tag, const size_t id) : m_id(id), m_tag(tag) {}

    public:
        std::shared_ptr<component::CTransform>      p_CTransform;
        std::shared_ptr<component::CShape>          p_CShape;
        std::shared_ptr<component::CCollision>      p_CCollision;
        std::shared_ptr<component::CInput>          p_CInput;
        std::shared_ptr<component::CScore>          p_CScore;
        std::shared_ptr<component::CLifespan>       p_CLifespan;
        
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
        /* update the vector every time frame -> add new entities and remove the dead ones */
        void    update();
    };
}
