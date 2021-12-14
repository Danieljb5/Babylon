#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow* window = new sf::RenderWindow();
    int winwidth = sf::VideoMode::getDesktopMode().width;
    int winheight = sf::VideoMode::getDesktopMode().height;
    window->create({winwidth, winheight}, "Tallest Towers", sf::Style::Fullscreen);

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
