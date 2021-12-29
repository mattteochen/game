/*
 * System is where the entity properties are updated, hence this part of the software manages the real WORK (i.e. movements, shoots ... )
 * */

namespace game_system
{
    /* entity position / movement update */
    class SMovement
    {
    public:
        void                movement(void *game);
    };
    
    /* user input */
    class SUserinput
    {
    public:
        void                input(void *game);
    };

    /* lifespan */
    class SLifespan
    {
    public:
        void                lifespan(void *game);
    };

    /* render / drawing */
    class SRender
    {
    public:
        void                render(void *game);
    };

    /* spawn enemies */
    class SEnemyspawner
    {
    public:
        void                spaw_enemy(void *game);
    };

    /* collisions */
    class SCollision
    {
    public:
        void                collision(void *game);
    };
}
