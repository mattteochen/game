#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "game.h"
#include <memory>
/* system.h is included in game.h */

void
game_system::SMovement::movement(void *game)
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

void
game_system::SLifespan::lifespan(void *game)
{
    game::Game *this_game = (game::Game*)game;

    /* decrease entities lifespan, if there had one */
    for (auto &e : this_game->m_entity_manager.getEntities())
    {
        if (!e->p_CLifespan) continue;
        else
        {
            if (e->p_CLifespan->remaining > 1)
            {
                e->p_CLifespan->remaining--;
                /* scale its color alpha channel */
                auto color = e->p_CShape->m_shape.getFillColor();
                int new_alpha = ALPHA_SCALING;
                sf::Color new_color(color.r, color.g, color.b, new_alpha);
                e->p_CShape->m_shape.setFillColor(new_color);
            }
            else if (e->p_CLifespan->remaining <= 1) e->destroy();
        }
    }
}

void
game_system::SCollision::collision(void *game)
{
    game::Game *this_game = (game::Game*)game;

    auto do_collide = [](const sf::Vector2f pos_player, const sf::Vector2f pos_enemy, const float collision_radius_sum)->bool
    {
        float x = abs(pos_enemy.x - pos_player.x);
        float y = abs(pos_enemy.y - pos_player.y);
        float powered_hy = pow(x,2) + pow(y,2);

        return (powered_hy <= pow(collision_radius_sum,2));
    };

    auto do_touch_walls = [&](const sf::Vector2f pos, const float radius)
    {
        return (pos.x <= 0 || pos.y <= 0 || pos.y >= (this_game->m_window_config.height-radius*2) || pos.x >= (this_game->m_window_config.width-radius*2));
    };

    auto recalculate_velocity = [&](std::shared_ptr<entity::Entity> &e)
    {
        if (e->p_CTransform->m_vel.x > 0 && e->p_CTransform->m_vel.y < 0)
        {
            e->p_CTransform->m_vel.x *= -1; 
        }
        else if (e->p_CTransform->m_vel.x < 0 && e->p_CTransform->m_vel.y < 0)
        {
            e->p_CTransform->m_vel.y *= -1;
        }
        else if (e->p_CTransform->m_vel.x < 0 && e->p_CTransform->m_vel.y > 0)
        {
            e->p_CTransform->m_vel.x *= -1;
        }
        else if (e->p_CTransform->m_vel.x > 0 && e->p_CTransform->m_vel.y > 0) 
        {
            e->p_CTransform->m_vel.y *= -1;
        }
        else
        {
            std::cerr << e->p_CTransform->m_vel.x << "  " << e->p_CTransform->m_vel.y << std::endl;
            std::cerr << "direction error\n";
            exit(EXIT_FAILURE);
        }
    };

    /* player - enemy collision */
    for (auto &p : this_game->m_entity_manager.getEntitiesTag("player"))
    {
        for (auto &e : this_game->m_entity_manager.getEntitiesTag("enemy"))
        {
            if (do_collide(p->p_CTransform->m_pos, e->p_CTransform->m_pos, p->p_CCollision->radius + e->p_CCollision->radius))
            {
                e->destroy();
                this_game->spawnPlayer();
            }
        }
    }
    /* bullet - enemy collision */
    for (auto &b : this_game->m_entity_manager.getEntitiesTag("bullet"))
    {
        for (auto &e : this_game->m_entity_manager.getEntitiesTag("enemy"))
        {
            if (do_collide(b->p_CTransform->m_pos, e->p_CTransform->m_pos, b->p_CCollision->radius + e->p_CCollision->radius))
            {
                e->destroy();
            }
        }
    }

    /* check the bounds */
    for (auto &b : this_game->m_entity_manager.getEntitiesTag("bullet"))
    {
        if (do_touch_walls(b->p_CTransform->m_pos, b->p_CShape->m_shape.getRadius()))
        {
            recalculate_velocity(b);
        }
    }
    for (auto &e : this_game->m_entity_manager.getEntitiesTag("enemy"))
    {
        if (do_touch_walls(e->p_CTransform->m_pos, e->p_CShape->m_shape.getRadius()))
        {
            recalculate_velocity(e);
        }
    }
    /* TODO: update player position */
}

void
game_system::SEnemyspawner::spaw_enemy(void *game)
{
    game::Game *this_game = (game::Game*)game;
    
    if (this_game->m_current_frame - this_game->m_last_enemy_spawn_time >= this_game->m_enemy_config.SI)
    {
        this_game->spawnEnemy();
    }
}

void
game_system::SRender::render(void *game)
{
    game::Game *this_game = (game::Game*)game;
    
    /* always clear the window */
    this_game->m_window.clear();

    /* TODO: add all the other entities */
    
    /* set position */
    this_game->m_player->p_CShape->m_shape.setPosition(this_game->m_player->p_CTransform->m_pos.x, this_game->m_player->p_CTransform->m_pos.y);
    /* set rotation */  
    this_game->m_player->p_CTransform->m_angle += 1.0f;
    this_game->m_player->p_CShape->m_shape.setRotation(this_game->m_player->p_CTransform->m_angle);

    /* sfml window draw */
    this_game->m_window.draw(this_game->m_player->p_CShape->m_shape);
    
    /* display */
    this_game->m_window.display();
}

void
game_system::SUserinput::input(void *game)
{
    game::Game *this_game = (game::Game*)game;
    
    sf::Event event;
    while (this_game->m_window.pollEvent(event))
    {
        /* window closed */
        if (event.type == sf::Event::Closed)
        {
            this_game->m_running_game = 0;
        }

        if (event.type == sf::Event::KeyPressed)
        {   
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                std::cout << "pressed W\n";
                this_game->m_player->p_CInput->up = 1;
                break;
            case sf::Keyboard::A:
                std::cout << "pressed A\n";
                this_game->m_player->p_CInput->left = 1;
                break;
            case sf::Keyboard::D:
                std::cout << "pressed D\n";
                this_game->m_player->p_CInput->right = 1;
                break;
            case sf::Keyboard::S:
                std::cout << "pressed S\n";
                this_game->m_player->p_CInput->down = 1;
                break;
            }
        }

        if (event.type == sf::Event::KeyReleased)
        {   
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                std::cout << "released W\n";
                this_game->m_player->p_CInput->up = 0;
                break;
            case sf::Keyboard::A:
                std::cout << "released A\n";
                this_game->m_player->p_CInput->left = 0;
                break;
            case sf::Keyboard::D:
                std::cout << "released D\n";
                this_game->m_player->p_CInput->right = 0;
                break;
            case sf::Keyboard::S:
                std::cout << "released S\n";
                this_game->m_player->p_CInput->down = 0;
                break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                std::cout << "spawn bullet\n";
                this_game->spawnBullets(this_game->m_player, sf::Vector2f(event.mouseButton.x,event.mouseButton.y));
            }
            
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                std::cout << "spawn special\n";
                /* TODO: special attack */
            }
        }
    }
}
