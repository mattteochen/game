#include "game.h"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <_types/_uint8_t.h>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <stddef.h>

void
populate_iniput(const std::string config, game::Game *game)
{
    std::ifstream file_in(config);
    std::string s;
    int num = 0;
    
    if (file_in.fail())
    {
        std::cerr << "config file error\n";
        exit(EXIT_FAILURE);
    }

    file_in >> s;
    if (s == "Window")
    {
        file_in >> game->m_window_config.width;
        file_in >> game->m_window_config.height;
        file_in >> game->m_window_config.frame_rate;
        file_in >> game->m_window_config.full_screen;
    }
    else
    {
        std::cerr << "file error window\n";
        exit(EXIT_FAILURE);
    }

    file_in >> s;
    if (s == "Font")
    {
        std::string font_path;
        file_in >> font_path;
        game->m_font.loadFromFile(font_path);
        
        int text_size;
        file_in >> text_size;
        game->m_text.setCharacterSize(text_size);

        std::vector<int> color(3,0);
        file_in >> color[0] >> color[1] >> color[2];
        game->m_text.setFillColor(sf::Color((uint8_t)color[0],(uint8_t)color[1],(uint8_t)color[2]));
    }
    else
    {
        std::cerr << "file error font\n";
        exit(EXIT_FAILURE);
    }


    file_in >> s;
    if (s == "Player")
    {
        file_in >> game->m_player_config.SR >> game->m_player_config.CR >> game->m_player_config.S >> game->m_player_config.FR >> game->m_player_config.FG >> game->m_player_config.FB >> game->m_player_config.OR >> game->m_player_config.OG >> game->m_player_config.OB >> game->m_player_config.OT >> game->m_player_config.V;
    }
    else
    {
        std::cerr << "file error player " << "\n";
        exit(EXIT_FAILURE);
    }   
        
    file_in >> s;
    if (s == "Enemy")
    {
        file_in >> game->m_enemy_config.SR >> game->m_enemy_config.CR >> game->m_enemy_config.SMIN >> game->m_enemy_config.SMAX >> game->m_enemy_config.OR >> game->m_enemy_config.OG >> game->m_enemy_config.OB >> game->m_enemy_config.OT >> game->m_enemy_config.VMIN >> game->m_enemy_config.VMAX >> game->m_enemy_config.L >> game->m_enemy_config.SI;
    }
    else
    {
        std::cerr << "file error enemy " << "\n";
        exit(EXIT_FAILURE);
    }   
    
    file_in >> s;
    if (s == "Bullet")
    {
        file_in >> game->m_bullet_config.SR >> game->m_bullet_config.CR >> game->m_bullet_config.S >>game->m_bullet_config.FR >> game->m_bullet_config.FG >> game->m_bullet_config.FB >> game->m_bullet_config.OR >> game->m_bullet_config.OG >> game->m_bullet_config.OB >> game->m_bullet_config.OT >> game->m_bullet_config.V >> game->m_bullet_config.L;
    }
    else
    {
        std::cerr << "file error bullet\n";
        exit(EXIT_FAILURE);
    }   
}

game::Game::Game(const std::string config)
{
    init(config);
}

void
game::Game::init(const std::string config)
{
    /* for random num */
    std::srand(time(NULL));
    
    populate_iniput(config, this);
    
    /* create the game window */
    m_window.create(sf::VideoMode(m_window_config.width,m_window_config.height), "Game!");
    m_window.setFramerateLimit(m_window_config.frame_rate);
    if (m_window_config.full_screen)
    {
        //TODO
    }

    /* load background */
    if (!m_texture.loadFromFile("../bkg.jpg"))
    {
        std::cerr << "image error\n";
        exit(EXIT_FAILURE);
    }
    /* calculate scale */
    float scaleX = (float)m_window_config.width / m_texture.getSize().x;
    float scaleY = (float)m_window_config.height / m_texture.getSize().y;
    m_sprite.setTexture(m_texture);
    m_sprite.setScale(scaleX, scaleY);

    spawnPlayer();
}

void
game::Game::spawnPlayer()
{   
    /* call the entity manager to create a new entity */
    auto new_e = m_entity_manager.addEntity("player");

    /* now we wanna add component to that entity */
    /* transform -> position and velocity */
    new_e->p_CTransform = std::make_shared<component::CTransform>(sf::Vector2f((float)m_window_config.width/2-(float)m_player_config.SR/2, (float)m_window_config.height/2-(float)m_player_config.SR/2), sf::Vector2f(m_player_config.S,m_player_config.S), 0.0);
    /* shape -> player shape */
    new_e->p_CShape     = std::make_shared<component::CShape>(m_player_config.SR, m_player_config.V, std::vector<uint8_t> {(uint8_t)m_player_config.FR,(uint8_t)m_player_config.FG,(uint8_t)m_player_config.FB}, std::vector<uint8_t> {(uint8_t)m_player_config.OR,(uint8_t)m_player_config.OG,(uint8_t)m_player_config.OB}, m_player_config.OT);
    /* input -> for user keyboard input */
    new_e->p_CInput     = std::make_shared<component::CInput>();
    /* collision */
    new_e->p_CCollision = std::make_shared<component::CCollision>(m_player_config.CR);

    /* assign the player pointer */
    m_player = new_e;
}

/* main game loop */
int
game::Game::run()
{
    /* some systems must run even when the game is paused */
    while (m_running_game)
    {
        /* update our entities every frame */
        m_entity_manager.update();
        
        /* we activate these systems only if the game is running */
        m_system_enemy_spawner.spaw_enemy((void*)this);
        m_system_user_input.input((void*)this);
        if (!m_pause_game) m_system_movement.movement((void*)this);
        if (!m_pause_game) m_system_collision.collision((void*)this);
        /* after collision */
        if (m_running_game == 0) return 1;
        m_system_render.render((void*)this);
        if (!m_pause_game) m_system_life_span.lifespan((void*)this);

        /* FIXME not sure this is the right position for this call */
        m_current_frame++;
    }
    return 0;
}

void
game::Game::setPaused()
{
    this->m_pause_game = 1;
}

void
game::Game::spawnEnemy()
{
    /* spawn an enemy randomly inside the window size */
    int width_max   = m_window_config.width;
    int height_max  = m_window_config.height;

    auto is_in_player_area = [](const sf::Vector2f spawn_pos, const sf::Vector2f player_pos, const float radius)
    {
        if (squared_euclidean_distance(spawn_pos, player_pos) <= pow(radius,2)) return 1;
        return 0;
    };
    
    sf::Vector2f enemy_pos;
    do
    {
        sf::Vector2f tmp_vec(generate_rand(0,width_max - m_enemy_config.SR), generate_rand(0,height_max - m_enemy_config.SR));
        enemy_pos = tmp_vec;
    } while (is_in_player_area(enemy_pos, m_player->p_CTransform->m_pos, m_player->p_CShape->m_shape.getRadius()));

    float enemy_vel = generate_rand(m_enemy_config.SMIN, m_enemy_config.SMAX);
    int enemy_shape_sides = generate_rand(m_enemy_config.VMIN, m_enemy_config.VMAX);
    std::vector<uint8_t> enemy_color(3,0);
    enemy_color[0] = generate_rand(0, 255);
    enemy_color[1] = generate_rand(0, 255);
    enemy_color[2] = generate_rand(0, 255);
    
    /* call the entity manager to create a new entity */
    auto new_e = m_entity_manager.addEntity("enemy");

    /* now we wanna add component to that entity */
    /* transform -> position and velocity */
    new_e->p_CTransform = std::make_shared<component::CTransform>(enemy_pos, sf::Vector2f(enemy_vel,enemy_vel), 0.0);
    /* shape -> player shape */
    new_e->p_CShape     = std::make_shared<component::CShape>(m_enemy_config.SR, enemy_shape_sides, enemy_color, enemy_color, m_enemy_config.OT);
    /* score -> enemy points value */
    new_e->p_CScore     = std::make_shared<component::CScore>(100); /* FIXME, hardcoded points value */
    /* collision radious */
    new_e->p_CCollision = std::make_shared<component::CCollision>(m_enemy_config.CR);

    /* update the last enemy spawn time */
    m_last_enemy_spawn_time = m_current_frame;
};

void
game::Game::spawnEnemy(std::shared_ptr<entity::Entity> &old_enemy, sf::Vector2f &vel)
{
    /* call the entity manager to create a new entity */
    auto new_e = m_entity_manager.addEntity("small_enemy");

    /* now we wanna add component to that entity */
    /* transform -> position and velocity */
    new_e->p_CTransform = std::make_shared<component::CTransform>(old_enemy->p_CTransform->m_pos, vel, 0.0); /* FIXME, hardcoded angle */
    /* shape -> player shape */
    new_e->p_CShape     = std::make_shared<component::CShape>(old_enemy->p_CShape->m_shape.getRadius() / 2, old_enemy->p_CShape->m_shape.getPointCount(), old_enemy->p_CShape->m_shape.getFillColor(), old_enemy->p_CShape->m_shape.getOutlineColor(), old_enemy->p_CShape->m_shape.getOutlineThickness());
    /* score -> enemy points value */
    new_e->p_CScore     = std::make_shared<component::CScore>(old_enemy->p_CScore->score * 2); 
    /* collision radious */
    new_e->p_CCollision = std::make_shared<component::CCollision>(old_enemy->p_CCollision->radius / 2);
    /* lifespan -> small enemy life span */
    new_e->p_CLifespan = std::make_shared<component::CLifespan>(this->m_enemy_config.L, this->m_enemy_config.L);
};

void                
game::Game::spawnSmallEnemies(std::shared_ptr<entity::Entity> entity)
{
    /* we must spawn n small enemies of half size and with double points */
    int enemy_num = entity->p_CShape->m_shape.getPointCount();

    /* TODO calculate the angle based on the sides count */
    std::vector<sf::Vector2f> spawn_angles{sf::Vector2f(1.0,0.0), sf::Vector2f(-1.0,0.0), sf::Vector2f(0.0,1.0), sf::Vector2f(0.0,-1.0)};
    
    auto calculate_velocity_vector = [&]()->void
    {
        switch(enemy_num)
        {
            case 3:
                spawn_angles.erase(spawn_angles.begin() + 1, spawn_angles.begin() + spawn_angles.size());
                spawn_angles.push_back(sf::Vector2f(-0.5,sqrt(3)/2));
                spawn_angles.push_back(sf::Vector2f(-0.5,-sqrt(3)/2));
                break;

            case 4:
                break;
            
            case 6:
                spawn_angles.erase(spawn_angles.begin() + 2, spawn_angles.begin() + spawn_angles.size());
                spawn_angles.push_back(sf::Vector2f(0.5,sqrt(3)/2));
                spawn_angles.push_back(sf::Vector2f(-0.5,sqrt(3)/2));
                spawn_angles.push_back(sf::Vector2f(0.5,-sqrt(3)/2));
                spawn_angles.push_back(sf::Vector2f(-0.5,sqrt(3)/2));
                break;

            case 8:
                spawn_angles.push_back(sf::Vector2f(1/(sqrt(2)),1/(sqrt(2))));
                spawn_angles.push_back(sf::Vector2f(-1/(sqrt(2)),1/(sqrt(2))));
                spawn_angles.push_back(sf::Vector2f(1/(sqrt(2)),-1/(sqrt(2))));
                spawn_angles.push_back(sf::Vector2f(-1/(sqrt(2)),-1/(sqrt(2))));
                break;

            default:
                /* this number do not spawn */
                return;
                break;
        }
    };
    
    calculate_velocity_vector();

    for (size_t i = 0; i < enemy_num; i++)
    {
        spawnEnemy(entity, spawn_angles[i]);
    }
}

void
game::Game::spawnBullets(std::shared_ptr<entity::Entity> entity, const sf::Vector2f mouse_pos)
{
    auto calculate_vector_sign = [](const sf::Vector2f &initial_pos, const sf::Vector2f &target_pos, sf::Vector2i &ret)->void
    {
        if (target_pos.x > initial_pos.x && target_pos.y > initial_pos.y) { ret.x = 1; ret.y = 1; }
        else if (target_pos.x > initial_pos.x && target_pos.y < initial_pos.y) { ret.x = 1; ret.y = -1; }
        else if (target_pos.x < initial_pos.x && target_pos.y < initial_pos.y) { ret.x = -1; ret.y = -1; }
        else if (target_pos.x < initial_pos.x && target_pos.y > initial_pos.y) { ret.x = -1; ret.y = 1; }
        else if (target_pos.x < initial_pos.x && target_pos.y == initial_pos.y) { ret.x = -1; ret.y = 0; }
        else if (target_pos.x > initial_pos.x && target_pos.y == initial_pos.y) { ret.x = 1; ret.y = 0; }
        else if (target_pos.x == initial_pos.x && target_pos.y < initial_pos.y) { ret.x = 0; ret.y = -1; }
        else if (target_pos.x == initial_pos.x && target_pos.y > initial_pos.y) { ret.x = 0; ret.y = 1; }
        else { ret.x = 0; ret.y = 0; }
    };
        
    auto calculate_velocity = [&](const sf::Vector2f target_pos, const sf::Vector2f initial_pos, const float speed)->sf::Vector2f
    {
        sf::Vector2f ret(0,0);
       
        /* initial sign of the vector from the player to the target */
        sf::Vector2i sign(1,1);
        calculate_vector_sign(initial_pos, target_pos, sign);
        
        /* base case: the bullet must go on the axes */
        if (sign.x == 0 || sign.y == 0)
        {
            ret.x = speed * sign.x;
            ret.y = speed * sign.y;
            return ret;
        }
        else
        {   
            /* calculate hypotenuse */
            const float hy = sqrt(pow(abs(initial_pos.x - target_pos.x),2) + pow(abs(initial_pos.y - target_pos.y), 2));
    
            /* calculate alpha (between x axes and hyp) */
            const float alpha = acos(abs(initial_pos.x - target_pos.x) / abs(hy));

            /* calculate the small triangle sides for the speed value */
            ret.x = speed * cos(alpha) * (float)sign.x;
            ret.y = speed * sin(alpha) * (float)sign.y;
            return ret;
        }
    };

    /* call the entity manager to create a new entity */
    auto new_e = m_entity_manager.addEntity("bullet");

    /* now we wanna add component to that entity */
    /* transform -> position and velocity */
    new_e->p_CTransform = std::make_shared<component::CTransform>(entity->p_CTransform->m_pos, calculate_velocity(mouse_pos, entity->p_CTransform->m_pos, this->m_bullet_config.S), 0.0); /* FIXME, hardcoded angle */
    /* shape -> player shape */
    new_e->p_CShape     = std::make_shared<component::CShape>(this->m_bullet_config.SR, this->m_bullet_config.V, std::vector<uint8_t> {(uint8_t)this->m_bullet_config.FR,(uint8_t)this->m_bullet_config.FG,(uint8_t)this->m_bullet_config.FB}, std::vector<uint8_t> {(uint8_t)this->m_bullet_config.OR,(uint8_t)this->m_bullet_config.OG,(uint8_t)this->m_bullet_config.OB}, this->m_bullet_config.OT);
    /* collision radious */
    new_e->p_CCollision = std::make_shared<component::CCollision>(this->m_bullet_config.CR);
    /* lifespan -> small enemy life span */
    new_e->p_CLifespan  = std::make_shared<component::CLifespan>(this->m_bullet_config.L, this->m_bullet_config.L);
}

/* TODO */
void                        
game::Game::spawnSpecialWeapon(std::shared_ptr<entity::Entity> entity)
{

}

int
game::Game::get_max_score(const int score)
{
    std::ifstream file("../score.txt");
    int old_score = 0;
    file >> old_score;

    if (old_score >= score)
    {
        file.close();
        return old_score;
    }
    else
    {
        file.close();
        std::ofstream file_o("../score.txt", std::ofstream::trunc);
        file_o << score;
        file_o.close();
        return score;
    }
}

void
game::Game::print_score(const int score)
{
    /* set the game over background */
    if (!m_texture.loadFromFile("../gover.jpg"))
    {
        std::cerr << "image error\n";
        exit(EXIT_FAILURE);
    }
    /* calculate scale */
    float scaleX = (float)m_window_config.width / m_texture.getSize().x;
    float scaleY = (float)m_window_config.height / m_texture.getSize().y;
    m_sprite.setTexture(m_texture);
    m_sprite.setScale(scaleX, scaleY);

    std::string score_str = "GAME OVER LOSER!\nSCORE: ";
    score_str += std::to_string(m_score) + "  ";
    score_str += "BEST SCORE: ";
    score_str += std::to_string(score);
    score_str += "\nR to respawn / E to exit";
    m_text.setString(score_str);
    m_text.setCharacterSize(100);
    m_text.setFont(m_font);
    sf::FloatRect text_size = m_text.getLocalBounds();
    m_text.setFillColor(sf::Color(0,204,0));
    m_text.setPosition((float)m_window_config.width/2 - text_size.width / 2, (float)m_window_config.height / 2 - text_size.height / 2);

    while(m_window.isOpen())
    {
        m_window.clear();
        /* The event handler is essential ! */
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) m_window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
            {
                return;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
            {
                exit(EXIT_SUCCESS);
            }
        }
        m_window.draw(m_sprite);
        m_window.draw(m_text);
        m_window.display();
    }
}
