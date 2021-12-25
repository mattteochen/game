#include "game.h"

void
populate_iniput(const std::string config, game::Game *game)
{
    std::ifstream file_in(config);
    std::string s;
    int num = 0;

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
        std::cerr << "file error\n";
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

        std::vector<uint8_t> color;
        file_in >> color[0] >> color[1] >> color[2];
        game->m_text.setFillColor(sf::Color(color[0],color[1],color[2]));
    }
    else
    {
        std::cerr << "file error\n";
        exit(EXIT_FAILURE);
    }


    file_in >> s;
    if (s == "Player")
    {
        file_in >> game->m_player_config.SR >> game->m_player_config.CR >> game->m_player_config.S >>game->m_player_config.FR >> game->m_player_config.FG >> game->m_player_config.FB >> game->m_player_config.OR >> game->m_player_config.OG >> game->m_player_config.OB >> game->m_player_config.OT >> game->m_player_config.V;
    }
    else
    {
        std::cerr << "file error\n";
        exit(EXIT_FAILURE);
    }   
        
    file_in >> s;
    if (s == "Enemy")
    {
        file_in >> game->m_enemy_config.SR >> game->m_enemy_config.CR >> game->m_enemy_config.SMIN >> game->m_enemy_config.SMAX >> game->m_enemy_config.OR >> game->m_enemy_config.OG >> game->m_enemy_config.OB >> game->m_enemy_config.OT >> game->m_enemy_config.VMIN >> game->m_enemy_config.VMAX >> game->m_enemy_config.L >> game->m_enemy_config.SI;
    }
    else
    {
        std::cerr << "file error\n";
        exit(EXIT_FAILURE);
    }   
    
    file_in >> s;
    if (s == "Bullet")
    {
        file_in >> game->m_bullet_config.SR >> game->m_bullet_config.CR >> game->m_bullet_config.S >>game->m_bullet_config.FR >> game->m_bullet_config.FG >> game->m_bullet_config.FB >> game->m_bullet_config.OR >> game->m_bullet_config.OG >> game->m_bullet_config.OB >> game->m_bullet_config.OT >> game->m_bullet_config.V >> game->m_bullet_config.L;
    }
    else
    {
        std::cerr << "file error\n";
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
    populate_iniput(config, this);

    /* create the game window */
    m_window.create(sf::VideoMode(m_window_config.width,m_window_config.height), "Game!");
    m_window.setFramerateLimit(m_window_config.frame_rate);
    if (m_window_config.full_screen)
    {
        //TODO
    }
    spawnPlayer();
}

void
game::Game::spawnPlayer()
{   
    /* call the entity manager to create a new entity */
    auto new_e = m_entity_manager.addEntity("player");

    /* now we wanna add component to that entity */
    /* transform -> position and velocity */
    new_e->p_CTransform = std::make_shared<component::CTransform>(sf::Vector2f(200.0,200.0), sf::Vector2f(m_player_config.S,m_player_config.S), 0.0);
    /* shape -> player shape */
    new_e->p_CShape     = std::make_shared<component::CShape>(m_player_config.SR, m_player_config.V, std::vector<uint8_t> {m_player_config.FR,m_player_config.FG,m_player_config.FB}, std::vector<uint8_t> {m_player_config.OR,m_player_config.OG,m_player_config.OB}, m_player_config.OT);
    /* input -> for user keyboard input */
    new_e->p_CInput     = std::make_shared<component::CInput>();

    /* assign the player pointer */
    m_player = new_e;
}

/* main game loop */
void
game::Game::run()
{

}
