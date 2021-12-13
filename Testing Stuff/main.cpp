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

        sf::View cam;
        cam.setCenter(0, 0);
        window->setView(cam);

        window->clear();

        sf::VertexArray va;
        va.setPrimitiveType(sf::Triangles);
        va.resize(3);
        sf::Vertex* v = &va[0];
        v->color = sf::Color(0xFF0000FF);
        v->position = {-200, 200};
        v = &va[1];
        v->color = sf::Color(0x00FF00FF);
        v->position = {200, 200};
        v = &va[2];
        v->color = sf::Color(0x0000FFFF);
        v->position = {0, -200};
        window->draw(&va[0], va.getVertexCount(), sf::Triangles);
        va.clear();
        
        window->display();
    }
    
    return 0;
}