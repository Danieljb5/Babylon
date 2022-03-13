#include <common.hpp>

#include <SFML/Graphics.hpp>
#include <iostream>

#ifndef DIST
    #include <debug.hpp>
#endif

std::unordered_map<uint64_t, entity> owned_entities;
std::unordered_map<uint64_t, entity> entities;
std::vector<tile*> all_tiles;
ut::StaticQuadTree<tile*> collision_tiles;

int main()
{
#ifndef DIST
    test_debug_version(global_version);
#endif
    sf::RenderWindow* window = new sf::RenderWindow();
    window->create({1920, 1080}, "smol", sf::Style::Fullscreen);
    sf::Image icon;
    if(!icon.loadFromFile("assets/icon.png"))
    {
        std::cout << "Could not load icon\n";
        exit(1);
    }
    window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    net::client_interface<message_types> client(global_version);
    client.connect("127.0.0.1", 36000);

    entity player_entity;
    player_entity.size = {50, 50};
    const size_t player = player_entity.id;
    owned_entities.insert({player_entity.id, player_entity});
    bool w = false, a = false, s = false, d = false;
    const float movement_speed = 500.0f;
    sf::Clock c;
    
    while(window->isOpen())
    {
        const float dt = c.restart().asSeconds();
        sf::Event event;
        while(window->pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window->close();
            }

            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::W) w = true;
                if(event.key.code == sf::Keyboard::A) a = true;
                if(event.key.code == sf::Keyboard::S) s = true;
                if(event.key.code == sf::Keyboard::D) d = true;
            }

            if(event.type == sf::Event::KeyReleased)
            {
                if(event.key.code == sf::Keyboard::W) w = false;
                if(event.key.code == sf::Keyboard::A) a = false;
                if(event.key.code == sf::Keyboard::S) s = false;
                if(event.key.code == sf::Keyboard::D) d = false;
            }
        }

        if(w) owned_entities[player].position.y -= movement_speed * dt;
        if(a) owned_entities[player].position.x -= movement_speed * dt;
        if(s) owned_entities[player].position.y += movement_speed * dt;
        if(d) owned_entities[player].position.x += movement_speed * dt;

        net::message<message_types> msg;
        msg.header.id = message_types::UPDATE_ENTITIES;
        auto it = owned_entities.begin();
        while(it != owned_entities.end())
        {
            msg << it->second;
            it++;
        }
        client.sendMessage(msg);

        while(!client.incoming().empty())
        {
            auto msg = client.incoming().pop_front().msg;
            
            switch(msg.header.id)
            {
                default:
                    break;

                case CHAT_MSG:
                {
                    break;
                }

                case UPDATE_ENTITIES:
                {
                    while(msg.body.size() > 0)
                    {
                        entity e;
                        msg >> e;
                        if(!entities.count(e.id))
                        {
                            entities.insert({e.id, e});
                        }
                        else
                        {
                            entities[e.id] = e;
                        }
                    }
                    break;
                }
            }
        }

        window->clear();

        it = entities.begin();
        while(it != entities.end())
        {
            sf::RectangleShape rect;
            rect.setPosition(it->second.position.get_vec());
            rect.setOrigin((it->second.size * 0.5).get_vec());
            rect.setSize(it->second.size.get_vec());
            rect.setFillColor(sf::Color::White);
            window->draw(rect);
            it++;
        }
        it = owned_entities.begin();
        while(it != owned_entities.end())
        {
            sf::RectangleShape rect;
            rect.setPosition(it->second.position.get_vec());
            rect.setOrigin((it->second.size * 0.5).get_vec());
            rect.setSize(it->second.size.get_vec());
            rect.setFillColor(sf::Color::White);
            window->draw(rect);
            it++;
        }
        if(!client.isConnected()) window->clear(sf::Color::Blue);

        window->display();
    }
}