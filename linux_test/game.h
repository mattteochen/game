/*
 * Game main definition
 * */

#include "system.h"
#include "entity.h"

namespace game
{
    typedef struct
    {
        int SR, CR, OT, V;
        int FR, FG, FB, OR, OG, OB;
        float S;
    } PLAYER_CONFIG;

    typedef struct
    {
        int SR, CR, OT, VMIN, VMAX, L, SI;
        int OR, OG, OB;
        float SMIN, SMAX;
    } ENEMY_CONFIG;

    typedef struct
    {
        int SR, CR, OT, V, L;
        int FR, FG, FB, OR, OG, OB;
        float S;
    } BULLET_CONFIG;

    typedef struct
    {
        int width, height, frame_rate, full_screen;
    } WINDOW_CONFIG;

    /* common lambdas */
    auto squared_euclidean_distance = [](const sf::Vector2f a, const sf::Vector2f b)
    {
        float ret = 0.0;
        float x = abs(a.x - b.x);
        float y = abs(a.y - b.y);
        return ret = (pow(x,2) + pow(y,2));
    };

    auto generate_rand = [](const int min_v, const int max_v)->float
    {
        float ret = 0;
        ret = rand() % (max_v-min_v+1) + min_v;
        return ret;
    };

    class Game
    {
        friend class game_system::SMovement;
        friend class game_system::SUserinput;
        friend class game_system::SRender;
        friend class game_system::SLifespan;
        friend class game_system::SEnemyspawner;
        friend class game_system::SCollision;

        sf::RenderWindow            m_window;
        sf::Texture                 m_texture;
        sf::Sprite                  m_sprite;
        int                         m_score = 0;
        int                         m_current_frame;
        int                         m_last_enemy_spawn_time = 0; /* to spawn enemy at a certain rate */
        bool                        m_pause_game = 0;
        bool                        m_running_game = 1;
                
        /* game entities */
        entity::Manager             m_entity_manager;
    
        /* game current player */
        std::shared_ptr<entity::Entity> m_player;
        
        /* game systems */
        game_system::SMovement       m_system_movement;
        game_system::SUserinput      m_system_user_input;
        game_system::SLifespan       m_system_life_span;
        game_system::SRender         m_system_render;
        game_system::SEnemyspawner   m_system_enemy_spawner;
        game_system::SCollision      m_system_collision;

        /* game private methods */
        void                        init(const std::string config);                                                     /* initialize game param from config file */
        void                        setPaused();                                                                        /* pause the game */
        void                        spawnPlayer();
        void                        spawnEnemy();
        void                        spawnEnemy(std::shared_ptr<entity::Entity> &old_enemy, sf::Vector2f &vel);
        void                        spawnSmallEnemies(std::shared_ptr<entity::Entity> entity);
        void                        spawnBullets(std::shared_ptr<entity::Entity> entity, const sf::Vector2f mouse_pos);
        void                        spawnSpecialWeapon(std::shared_ptr<entity::Entity> entity);

    public:
        sf::Text                    m_text;
        sf::Font                    m_font;
        ENEMY_CONFIG                m_enemy_config;
        BULLET_CONFIG               m_bullet_config;
        PLAYER_CONFIG               m_player_config;
        WINDOW_CONFIG               m_window_config;

        Game(const std::string config);
        int                 run();
        int                 get_max_score(const int score);
        void                print_score(const int score);
    };

}
