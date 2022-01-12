#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

double mindt = 0xFFFFFFF, avgdt = 0, maxdt = 0;
std::vector<double> dts;

void printStats()
{
    int i;
    for(i = 1000; i < dts.size() - 1000; i++)
    {
        avgdt += dts[i];
        if(dts[i] < mindt) mindt = dts[i];
        if(dts[i] > maxdt) maxdt = dts[i];
    }
    avgdt /= i;

    std::cout << "Max FPS: " << (int)(1.f / mindt) << std::endl << "Avg FPS: " << (int)(1.f / avgdt) << std::endl << "Min FPS: " << (int)(1.f / maxdt) << std::endl;
}

int main()
{
    atexit(printStats);

    sf::RenderWindow* window = new sf::RenderWindow();
    int winwidth = sf::VideoMode::getDesktopMode().width;
    int winheight = sf::VideoMode::getDesktopMode().height;
    window->create({winwidth, winheight}, "Tallest Towers", sf::Style::Fullscreen);
    sf::Clock clock;
    double minTimer = 0.f;

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

        double dt = clock.restart().asSeconds();

        minTimer += dt;

        if(minTimer <= 5.f) continue;
        if(minTimer >= 16.f)
        {
            window->close();
            break;
        }
        if(minTimer >= 15.f) continue;

        dts.push_back(dt);
    }

    delete window;
    
    return 0;
}
