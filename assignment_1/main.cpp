#include <cstdlib>
#include <iostream>
#include "../SFML-2.5.1-macos-clang/include/SFML/Graphics.hpp"
#include "../common/config.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"

int main(int args, char *argv[])
{
    /* create the window ------------------------------------------------------------------------ */
    sf::RenderWindow window(sf::VideoMode(WINDOW_LENGTH, WINDOW_HEIGH), "My Window");
    
    /* create a figure -------------------------------------------------------------------------- */
    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color(255,0,255));
    circle.setPosition(300.0f, 300.0f);
    float circleMoveSpeed = -0.01f;
    
    /* regular polygon are shaped by CircleShape -> just specify the num of sides */
    sf::CircleShape triangle(50, 3); 
    triangle.setFillColor(sf::Color(51,255,51));
    triangle.setPosition(300.0f, 300.0f);
    float triangleMoveSpeed = 0.01f;

    /* let's draw some fonts -------------------------------------------------------------------- */
    sf::Font myFont;
    /* try to load a font from file */
    if (!myFont.loadFromFile(FONT_DIR))
    {
        std::cerr << "Can't load font!\n";
        exit(EXIT_FAILURE);
    }
    /* set up the font that we wanna display */
    sf::Text testText("Hello World!", myFont, FONT_SIZE);
    /* (x, y) */
    testText.setPosition(0, WINDOW_HEIGH - (float)testText.getCharacterSize());

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

                /* move a shape */
                if (event.key.code == sf::Keyboard::X) 
                {
                    /* reverse the animation */
                    circleMoveSpeed *= -1.0f;
                }
                if (event.key.code == sf::Keyboard::Y) 
                {
                    /* reverse the animation */
                    triangleMoveSpeed *= -1.0f;
                }
            }
        }

        /* basic animation */
        circle.setPosition(circle.getPosition() + sf::Vector2f(circleMoveSpeed, circleMoveSpeed));
        triangle.setPosition(triangle.getPosition() + sf::Vector2f(triangleMoveSpeed, triangleMoveSpeed));
        
        window.clear();
        window.draw(circle);
        window.draw(triangle);
        window.draw(testText);
        window.display();
    }

    return 0;
}
