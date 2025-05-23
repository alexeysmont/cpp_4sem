#include <SFML/Graphics.hpp>

#include "Game.h"
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(900, 700)), "Gems");
    GameTable game_table(12, 9);
    
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                std::cout << "the left button was pressed" << keyPressed->position.x << std::endl;
                game_table.on_click({keyPressed->position.x, keyPressed->position.y});
                
            }
        }
        game_table.paint_black(window);
        game_table.paint(window);
        game_table.check_single_color_sequences();
        window.display();
    }

    return 0;
}
