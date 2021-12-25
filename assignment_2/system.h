/*
 * System is where the entity properties are updated, hence this part of the software manages the real WORK (i.e. movements, shoots ... )
 * */

#include "entity.h"

namespace game_system
{
    /* entity position / movemnt update */
    class SMovement
    {
    public:
        void                movement();
    };
    
    /* user input */
    class SUserinput
    {
    public:
        void                input();
    };

    /* lifespan */
    class SLifespan
    {
    public:
        void                lifespan();
    };

    /* render / drawing */
    class SRender
    {
    public:
        void                render();
    };

    /* spawn enemies */
    class SEnemyspawner
    {
    public:
        void                spaw_enemy();
    };

    /* collisions */
    class SCollision
    {
    public:
        void                collision();
    };
}
