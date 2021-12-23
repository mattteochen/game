#include "figure_list.h"
#include <_types/_uint8_t.h>

int main(int args, char *argv[])
{
    /* create the window ------------------------------------------------------------------------ */
    sf::RenderWindow window(sf::VideoMode(WINDOW_LENGTH, WINDOW_HEIGH), "My Window");
    
    /* call the figure creation API */
    std::vector<create_figure::circleSfmlObject*> figure_list;
    create_figures(figure_list);
    
    /* Driver loop ------------------------------------------------------------------------------ */
    while(window.isOpen())
    {
        /* The event handler is essential ! */
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                std::cout << "Key pressed with code " << event.key.code << std::endl;
            }
        }

        auto calculate_new_direction = [&](const std::pair<int,int> dir) -> const std::pair<int,int>
        {
            if (dir.first > 0 && dir.second < 0)
            {
                return std::make_pair(dir.first * -1, dir.second);
            }
            else if (dir.first < 0 && dir.second < 0)
            {
                return std::make_pair(dir.first, dir.second * -1);
            }
            else if (dir.first < 0 && dir.second > 0)
            {
                return std::make_pair(dir.first * -1, dir.second);
            }
            else if (dir.first > 0 && dir.second > 0) 
            {
                return std::make_pair(dir.first, dir.second * -1);
            }
            else
            {
                std::cout << dir.first << "  " << dir.second << std::endl;
                std::cerr << "direction error\n";
                exit(EXIT_FAILURE);
            }
        };
        
        for (auto &p : figure_list)
        {
            (*p).setPosition(sf::Vector2f((float)(*p).getDirection().first / 10, (float)(*p).getDirection().second / 10));
            if (((*p).circleFigure.getPosition().y <= 0) ||
                ((*p).circleFigure.getPosition().x <= 0) ||
                ((*p).getPosition().x >= window.getSize().x - ((*p).circleFigure.getRadius() * 2)) ||
                ((*p).getPosition().y >= window.getSize().y - ((*p).circleFigure.getRadius() * 2)))
            {
                /* basic animation, the circle must change direction when touch the walls */
                std::pair<int,int> new_position = calculate_new_direction((*p).getDirection()); 
                (*p).setDirection(new_position);

                /* change colour */
                std::vector<uint8_t> old_colour = (*p).getRGB();
                for (auto &n : old_colour)
                {
                    n += 10;
                    if (n > 255) n -= 255;
                }
                (*p).circleFigure.setFillColor(sf::Color(old_colour[0], old_colour[1], old_colour[2]));
                (*p).setRGB(old_colour);
            }
        }

        window.clear();
        for (auto &p : figure_list)
        {
            window.draw((*p).circleFigure);
            window.draw((*p).circleFigureText);
        }
        window.display();
    }

    return 0;
}
