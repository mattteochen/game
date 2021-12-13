#include <iostream>
#include "../SFML-2.5.1-macos-clang/include/SFML/Graphics.hpp"
#include "../common/config.h"


int main(int args, char *argv[])
{
    /* create the window */
    sf::RenderWindow window(sf::VideoMode(WINDOW_LENGTH, WINDOW_HEIGH), "My Window");

    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Red);
    circle.setPosition(300.0f, 300.0f);
    float circleMoveSpeed = -0.01f;

    while(window.isOpen())
    {
        //sf::Event event;
        //while (window.pollEvent(event))
        //{
        //    if (event.type == sf::Event::Closed) window.close();
        //    
        //}

        /* basic animation */
        circle.setPosition(circle.getPosition() + sf::Vector2f(circleMoveSpeed, circleMoveSpeed));

        window.clear();
        window.draw(circle);
        window.display();
    }

    return 0;
}
