#include "figure_list.h"

sf::Font myFont;
create_figure::circleSfmlObject circle(CIRCLE_SIZE, sf::Vector2f(0.0f, 0.0f), CIRCLE_SPEED, std::vector<uint8_t> {65,32,56});  
create_figure::circleSfmlObject octagon(CIRCLE_SIZE, 8, sf::Vector2f(190.0f, 78.0f), CIRCLE_SPEED, std::vector<uint8_t> {98,23,43});  
create_figure::circleSfmlObject another_circle(CIRCLE_SIZE, sf::Vector2f(400.0f,270.0f), CIRCLE_SPEED * 2, std::vector<uint8_t> {156,98,43});

void
create_figures(std::vector<create_figure::circleSfmlObject*> &figure_list)
{
    /* let's draw some fonts -------------------------------------------------------------------- */
    /* try to load a font from file */
    if (!myFont.loadFromFile(FONT_DIR))
    {
        std::cerr << "Can't load font!\n";
        exit(EXIT_FAILURE);
    }

    /* create a figure -------------------------------------------------------------------------- */
    circle.circleFigure.setFillColor(sf::Color(65,32,56));
    circle.setText("Circle!", myFont, FONT_SIZE);
    /* FIXME: rgb setting in constructor do not work */
    figure_list.push_back(&circle);
    
    /* create a figure */
    octagon.circleFigure.setFillColor(sf::Color(98,23,43));
    octagon.setText("Octagon!", myFont, FONT_SIZE);
    figure_list.push_back(&octagon);
    
    /* create a figure */
    another_circle.circleFigure.setFillColor(sf::Color(156,98,43));
    another_circle.setText("Circle 2!", myFont, FONT_SIZE);
    figure_list.push_back(&another_circle);
}
