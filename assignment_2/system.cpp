#include "game.h"
/* system.h is included in game.h */

void game_system::SMovement::movement(void *game)
{
    game::Game *this_game = (game::Game*)game;
    sf::Vector2f player_vel(0,0);

    /* set the player velocity based on the input */
    if (this_game->m_player->p_CInput->up)
    {
        player_vel.y -= this_game->m_player_config.S;
    }
    if (this_game->m_player->p_CInput->down)
    {
        player_vel.y += this_game->m_player_config.S;
    }
    if (this_game->m_player->p_CInput->left)
    {
        player_vel.x -= this_game->m_player_config.S;
    }
    if (this_game->m_player->p_CInput->right)
    {
        player_vel.x += this_game->m_player_config.S;
    }

    /* update the player vel */
    this_game->m_player->p_CTransform->m_vel = player_vel;

    /* update the movement for all entities in the game after calculated the player new velocity */
    for (auto &e : this_game->m_entity_manager.getEntities())
    {
        e->p_CTransform->m_pos += e->p_CTransform->m_vel;
    }
}
