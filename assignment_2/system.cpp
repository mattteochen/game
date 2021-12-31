#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"
#include "game.h"
#include <_types/_uint8_t.h>
#include <cstdio>
#include <cstdlib>
#include <memory>
/* system.h is included in game.h */

#define DESTROY     1
#define NOT_DESTROY 0

bool do_collide(const sf::Vector2f pos_player, const sf::Vector2f pos_enemy, const float collision_radius_sum)
{
    float x = abs(pos_enemy.x - pos_player.x);
    float y = abs(pos_enemy.y - pos_player.y);
    float powered_hy = pow(x,2) + pow(y,2);
    return (powered_hy <= pow(collision_radius_sum,2));
}

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
        e->p_CTransform->m_pos.x += e->p_CTransform->m_vel.x;
        e->p_CTransform->m_pos.y += e->p_CTransform->m_vel.y;
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

    auto do_touch_walls = [&](std::shared_ptr<entity::Entity> &e, uint8_t code)
    {
        if (e->p_CTransform->m_pos.x < (0+e->p_CShape->m_shape.getRadius()) || this_game->m_window_config.width-e->p_CShape->m_shape.getRadius() < e->p_CTransform->m_pos.x)
        {
            if (code == DESTROY)
            {
                e->destroy();
                return;
            }
            e->p_CTransform->m_vel.x *= -1.0;
        }
        if (e->p_CTransform->m_pos.y < (0+e->p_CShape->m_shape.getRadius()) || this_game->m_window_config.height-e->p_CShape->m_shape.getRadius() < e->p_CTransform->m_pos.y)
        {
            if (code == DESTROY)
            {
                e->destroy();
                return;
            }
            e->p_CTransform->m_vel.y *= -1.0;
        }
    };

    /* player - enemy collision */
    for (auto &p : this_game->m_entity_manager.getEntitiesTag("player"))
    {
        for (auto &e : this_game->m_entity_manager.getEntitiesTag("enemy"))
        {
            if (do_collide(this_game->m_player->p_CTransform->m_pos, e->p_CTransform->m_pos, this_game->m_player->p_CCollision->radius + e->p_CCollision->radius))
            {
                int score = this_game->get_max_score(this_game->m_score);
                this_game->print_score(score);
                this_game->m_running_game = 0;
                return;
            }
        }
    }
    /* player - small enemy collision */
    for (auto &p : this_game->m_entity_manager.getEntitiesTag("player"))
    {
        for (auto &e : this_game->m_entity_manager.getEntitiesTag("small_enemy"))
        {
            if (do_collide(this_game->m_player->p_CTransform->m_pos, e->p_CTransform->m_pos, this_game->m_player->p_CCollision->radius + e->p_CCollision->radius))
            {
                int score = this_game->get_max_score(this_game->m_score);
                this_game->print_score(score);
                this_game->m_running_game = 0;
                return;
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
                b->destroy();
                e->destroy();
                this_game->spawnSmallEnemies(e);
                /* update the score */
                this_game->m_score += e->p_CScore->score;
                int new_divider = this_game->m_score/SPEACIAL_WEAPON_SCORE;
                if (new_divider > 0 && new_divider > this_game->m_previous_special_weapon_divider)
                {
                    this_game->m_special_weapon_num++;
                    this_game->m_previous_special_weapon_divider = new_divider;
                }
            }
        }
    }

    /* bullet - small enemy collision */
    for (auto &b : this_game->m_entity_manager.getEntitiesTag("bullet"))
    {
        for (auto &e : this_game->m_entity_manager.getEntitiesTag("small_enemy"))
        {
            if (do_collide(b->p_CTransform->m_pos, e->p_CTransform->m_pos, b->p_CCollision->radius + e->p_CCollision->radius))
            {
                b->destroy();
                e->destroy();
                /* update the score */
                this_game->m_score += e->p_CScore->score;
            }
        }
    }

    /* special bullet enemy collision */
    for (auto &b : this_game->m_entity_manager.getEntitiesTag("special"))
    {
        for (auto &e : this_game->m_entity_manager.getEntitiesTag("enemy"))
        {
            if (do_collide(b->p_CTransform->m_pos, e->p_CTransform->m_pos, b->p_CCollision->radius + e->p_CCollision->radius))
            {
                b->destroy();
                e->destroy();
                this_game->spawnSmallEnemies(e);
                /* update the score */
                this_game->m_score += e->p_CScore->score;
            }
        }
        for (auto &e : this_game->m_entity_manager.getEntitiesTag("small_enemy"))
        {
            if (do_collide(b->p_CTransform->m_pos, e->p_CTransform->m_pos, b->p_CCollision->radius + e->p_CCollision->radius))
            {
                b->destroy();
                e->destroy();
                /* update the score */
                this_game->m_score += e->p_CScore->score;
            }
        }
    }


    /* check the bounds */
    for (auto &b : this_game->m_entity_manager.getEntitiesTag("special"))
    {
        do_touch_walls(b, DESTROY);
    }
    for (auto &b : this_game->m_entity_manager.getEntitiesTag("bullet"))
    {
        do_touch_walls(b, NOT_DESTROY);
    }
    for (auto &e : this_game->m_entity_manager.getEntitiesTag("enemy"))
    {
        do_touch_walls(e, NOT_DESTROY);
    }
    for (auto &e : this_game->m_entity_manager.getEntitiesTag("small_enemy"))
    {
        do_touch_walls(e, NOT_DESTROY);
    }

    /* check player over bounds */
    if (this_game->m_player->p_CTransform->m_pos.x < (0+this_game->m_player->p_CShape->m_shape.getRadius()/2))
    {
        this_game->m_player->p_CTransform->m_pos.x = 0+this_game->m_player->p_CShape->m_shape.getRadius()/2;
    }
    if ((this_game->m_window_config.width-this_game->m_player->p_CShape->m_shape.getRadius()/2) < this_game->m_player->p_CTransform->m_pos.x)
    {
        this_game->m_player->p_CTransform->m_pos.x = this_game->m_window_config.width-this_game->m_player->p_CShape->m_shape.getRadius()/2;
    }
    if (this_game->m_player->p_CTransform->m_pos.y < (0+this_game->m_player->p_CShape->m_shape.getRadius()/2))
    {
        this_game->m_player->p_CTransform->m_pos.y = 0+this_game->m_player->p_CShape->m_shape.getRadius()/2;
    }
    if ((this_game->m_window_config.height-this_game->m_player->p_CShape->m_shape.getRadius()/2) < this_game->m_player->p_CTransform->m_pos.y)
    {
        this_game->m_player->p_CTransform->m_pos.y = this_game->m_window_config.height-this_game->m_player->p_CShape->m_shape.getRadius()/2;
    }
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

    this_game->m_window.draw(this_game->m_sprite);
    
    /* set position */
    this_game->m_player->p_CShape->m_shape.setPosition(this_game->m_player->p_CTransform->m_pos.x, this_game->m_player->p_CTransform->m_pos.y);
    /* set rotation */  
    this_game->m_player->p_CTransform->m_angle += 1.0f;
    this_game->m_player->p_CShape->m_shape.setRotation(this_game->m_player->p_CTransform->m_angle);

    /* sfml window draw */
    this_game->m_window.draw(this_game->m_player->p_CShape->m_shape);

    /* display bullet */
    for (auto &b : this_game->m_entity_manager.getEntitiesTag("bullet"))
    {
        b->p_CShape->m_shape.setPosition(b->p_CTransform->m_pos.x, b->p_CTransform->m_pos.y);
        b->p_CTransform->m_angle += 1.0f;
        b->p_CShape->m_shape.setRotation(b->p_CTransform->m_angle);
        this_game->m_window.draw(b->p_CShape->m_shape);
    }
    /* display special bullet */
    for (auto &b : this_game->m_entity_manager.getEntitiesTag("special"))
    {
        b->p_CShape->m_shape.setPosition(b->p_CTransform->m_pos.x, b->p_CTransform->m_pos.y);
        b->p_CTransform->m_angle += 1.0f;
        b->p_CShape->m_shape.setRotation(b->p_CTransform->m_angle);
        this_game->m_window.draw(b->p_CShape->m_shape);
    }
    /* display enemy */
    if (this_game->m_entity_manager.getEntitiesTag("enemy").size() > 0)
    {
        for (auto &e : this_game->m_entity_manager.getEntitiesTag("enemy"))
        {
            e->p_CShape->m_shape.setPosition(e->p_CTransform->m_pos.x, e->p_CTransform->m_pos.y);
            e->p_CTransform->m_angle += 1.0f;
            e->p_CShape->m_shape.setRotation(e->p_CTransform->m_angle);
            this_game->m_window.draw(e->p_CShape->m_shape);
        }
    }
    /* display small enemy */
    if (this_game->m_entity_manager.getEntitiesTag("small_enemy").size() > 0)
    {
        for (auto &e : this_game->m_entity_manager.getEntitiesTag("small_enemy"))
        {
            e->p_CShape->m_shape.setPosition(e->p_CTransform->m_pos.x, e->p_CTransform->m_pos.y);
            e->p_CTransform->m_angle += 1.0f;
            e->p_CShape->m_shape.setRotation(e->p_CTransform->m_angle);
            this_game->m_window.draw(e->p_CShape->m_shape);
        }
    }
    
    /* score and special weapons */
    std::string score_str = "SCORE: ";
    score_str += std::to_string(this_game->m_score);
    score_str += "  SPECIAL: ";
    score_str += std::to_string(this_game->m_special_weapon_num);
    this_game->m_text.setString(score_str);
    this_game->m_text.setCharacterSize(40);
    this_game->m_text.setFont(this_game->m_font);
    this_game->m_text.setPosition(0.0,0.0);
    this_game->m_window.draw(this_game->m_text);

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
                //std::cout << "pressed W\n";
                this_game->m_player->p_CInput->up = 1;
                break;
            case sf::Keyboard::A:
                //std::cout << "pressed A\n";
                this_game->m_player->p_CInput->left = 1;
                break;
            case sf::Keyboard::D:
                //std::cout << "pressed D\n";
                this_game->m_player->p_CInput->right = 1;
                break;
            case sf::Keyboard::S:
                //std::cout << "pressed S\n";
                this_game->m_player->p_CInput->down = 1;
                break;
            }
        }

        if (event.type == sf::Event::KeyReleased)
        {   
            switch (event.key.code)
            {
            case sf::Keyboard::W:
                //std::cout << "released W\n";
                this_game->m_player->p_CInput->up = 0;
                break;
            case sf::Keyboard::A:
                //std::cout << "released A\n";
                this_game->m_player->p_CInput->left = 0;
                break;
            case sf::Keyboard::D:
                //std::cout << "released D\n";
                this_game->m_player->p_CInput->right = 0;
                break;
            case sf::Keyboard::S:
                //std::cout << "released S\n";
                this_game->m_player->p_CInput->down = 0;
                break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                this_game->m_player->p_CInput->shoot = 1;
                this_game->spawnBullets(this_game->m_player, sf::Vector2f(event.mouseButton.x,event.mouseButton.y));
            }
            
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                if (this_game->m_special_weapon_num > 0)
                {
                    this_game->m_special_weapon_num--;
                    this_game->spawnSpecialWeapon();
                }
            }
        }
    }
}
