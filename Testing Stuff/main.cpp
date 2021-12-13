#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow* window = new sf::RenderWindow();
    window->create({800, 600}, "My Window", sf::Style::Default);

    while(window->isOpen())
    {
        sf::Event e;
        while(window->pollEvent(e))
        {
            if(e.type == sf::Event::Closed)
            {
                window->close();
                delete window;
                break;
            }
        }

        window->clear();
        window->display();
    }
    
    return 0;
}