#include "../common/create_figure.h"
#include "SFML/System/Vector2.hpp"

int main(int args, char *argv[])
{
    /* calculate the font pos bases on the object pos */
    auto calculate_centered_font = [&](const sf::Vector2f &object_pos, sf::Vector2f &font_pos, const float object_size, const float font_size)
    {
        float diff = abs(object_size - font_size); /* do not div 2 for circles */

        font_pos.x = object_pos.y + diff;
        /* TODO, x position */
        font_pos.x = object_pos.y;
    };

    /* create the window ------------------------------------------------------------------------ */
    sf::RenderWindow window(sf::VideoMode(WINDOW_LENGTH, WINDOW_HEIGH), "My Window");
    
    /* create a figure -------------------------------------------------------------------------- */
    std::vector<uint8_t> testRGB{100, 0, 255};
    sf::Vector2f testPos(CIRCLE_POS, CIRCLE_POS);
    create_figure::circleSfmlObject circle(CIRCLE_SIZE, testPos, CIRCLE_SPEED, testRGB);  
    /* FIXME: rgb setting in constructor do not work */

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
    sf::Vector2f testTextPosition;
    calculate_centered_font(circle.getPosition(), testTextPosition, circle.circleFigure.getRadius(), FONT_SIZE);
    testText.setPosition(testTextPosition.x, testTextPosition.y);
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
                    circle.setMoveSpeed(circle.getMoveSpeed() * -1.0f);
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
        circle.setPosition(circle.circleFigure.getPosition() + sf::Vector2f(circle.getMoveSpeed(), circle.getMoveSpeed()));
        testText.setPosition(testText.getPosition() + sf::Vector2f(testTextMoveSpeed, testTextMoveSpeed));
        
        window.clear();
        window.draw(circle.circleFigure);
        //window.draw(triangle);
        window.draw(testText);
        window.display();
    }

    return 0;
}
