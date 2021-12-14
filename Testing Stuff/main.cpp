#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow* window = new sf::RenderWindow();
    int winwidth = sf::VideoMode::getDesktopMode().width;
    int winheight = sf::VideoMode::getDesktopMode().height;
    window->create({winwidth, winheight}, "Tallest Towers", sf::Style::Fullscreen);
    sf::Clock clock;
    float mindt = 0xFFFFFFF, avgdt = 0, maxdt = 0;
    std::vector<float> dts;
    float minTimer = 0.f;

    while(window->isOpen())
    {
        sf::Event event;
        while(window->pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window->close();
                break;
            }
        }
        
        window->clear();
        
        window->display();

        float dt = clock.restart().asSeconds();

        minTimer += dt;

        if(minTimer <= 5.f) continue;
        if(minTimer >= 16.f)
        {
            window->close();
            break;
        }
        if(minTimer >= 15.f) continue;

        dts.push_back(dt);
        if(dt < mindt) mindt = dt;
        if(dt > maxdt) maxdt = dt;
    }

    int i;
    for(i = 0; i < dts.size(); i++)
    {
        avgdt += dts[i];
    }
    avgdt /= i;

    std::cout << (int)(1.f / mindt) << std::endl << (int)(1.f / avgdt) << std::endl << (int)(1.f / maxdt) << std::endl;

    delete window;
    
    return 0;
}
