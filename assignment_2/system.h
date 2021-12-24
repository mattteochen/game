/*
 * System is where the entity properties are updated, hence this part of the software manages the real WORK (i.e. movements, shoots ... )
 * */

#include "SFML/Graphics/Font.hpp"
#include "common.h"
#include "entity.h"
#include <memory>

namespace system
{
    typedef struct
    {
        int sr, cr, fr, fg, fb, or, og, ob, ot, v;
        float s;
    } PLAYER_CONFIG;

    typedef struct
    {
        int sr, cr , or, og, ob, ot, v_min, v_max, l, si;
        float s_min, s_max;
    } ENEMY_CONFIG;

    typedef struct
    {
        int sr, cr, fr, fg, fb, or, og, ob, ot, v;
        float s;
    } BULLET_CONFIG;

    class Game
    {
        sf::RenderWindow    m_window;
        sf::Text            m_text;
        sf::Font            m_font;
        entity::Manager     m_entity_manager;
        int                 m_score = 0;
        int                 m_current_frame;
        int                 m_last_enemy_spawn_time = 0; /* to spawn enemy at a certain rate */
        bool                m_pause_game = 0;
        bool                m_running_game = 1;
        ENEMY_CONFIG        m_enemy_config;
        BULLET_CONFIG       m_bullet_config;
        PLAYER_CONFIG       m_player_config;
        std::shared_ptr<entity::Entity> m_player;

        //TODO add private and public methods

    public:

    };


}
