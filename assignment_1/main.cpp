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
    /* calculate the font pos bases on the object pos */
    auto calculate_centered_font = [&](std::pair<float,float> &object_pos, std::pair<float,float> &font_pos, float object_size, float font_size)
    {
        float diff = abs(object_size - font_size); /* do not div 2 for circles */

        font_pos.second = object_pos.second + diff;
        /* TODO, x position */
        font_pos.first = object_pos.first;
    };

    /* create the window ------------------------------------------------------------------------ */
    sf::RenderWindow window(sf::VideoMode(WINDOW_LENGTH, WINDOW_HEIGH), "My Window");
    
    /* create a figure -------------------------------------------------------------------------- */
    std::pair<float,float> circlePosition({CIRCLE_POS, CIRCLE_POS});
    sf::CircleShape circle(CIRCLE_SIZE);
    circle.setFillColor(sf::Color(255,0,255));
    circle.setPosition(circlePosition.first, circlePosition.second);
    float circleMoveSpeed = CIRCLE_SPEED;
    
    /* regular polygon are shaped by CircleShape -> just specify the num of sides */
    //sf::CircleShape triangle(200, 3); 
    //triangle.setFillColor(sf::Color(51,255,51));
    //triangle.setPosition(300.0f, 300.0f);
    //float triangleMoveSpeed = 0.01f;

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
    std::pair<float,float> testTextPosition;
    calculate_centered_font(circlePosition, testTextPosition, CIRCLE_SIZE, FONT_SIZE);
    testText.setPosition(testTextPosition.first, testTextPosition.second);
    float testTextMoveSpeed = CIRCLE_SPEED;

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
                    testTextMoveSpeed *= -1.0f;
                }
                //if (event.key.code == sf::Keyboard::Y) 
                //{
                //    /* reverse the animation */
                //    triangleMoveSpeed *= -1.0f;
                //}
            }
        }

        /* basic animation */
        circle.setPosition(circle.getPosition() + sf::Vector2f(circleMoveSpeed, circleMoveSpeed));
        testText.setPosition(testText.getPosition() + sf::Vector2f(testTextMoveSpeed, testTextMoveSpeed));
        
        window.clear();
        window.draw(circle);
        //window.draw(triangle);
        window.draw(testText);
        window.display();
    }

    return 0;
}
