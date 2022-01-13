#include <iostream>
#include <vector>
#include <cstdint>
#include <SFML/Graphics.hpp>

std::pair<double, double> lerp(std::pair<double, double> start, std::pair<double, double> end, float percent)
{
    std::pair<double, double> delta;
    delta.first = (end.first - start.first) * percent;
    delta.second = (end.second - start.second) * percent;
    return {start.first + delta.first, start.second + delta.second};
}

struct obj
{
    std::pair<double, double> position = {0.0, 0.0};
    std::pair<double, double> half_size = {0.5, 0.5};

    std::pair<double, double> velocity = {0.0, 0.0};

    uint32_t colour = 0xFFFFFFFF;
    bool isGrounded = true;
    bool isStatic = true;

    bool isColliding(obj &other)
    {
        return !(position.first - half_size.first > other.position.first + other.half_size.first || position.first + half_size.first < other.position.first - other.half_size.first || position.second - half_size.second > other.position.second + other.half_size.second || position.second + half_size.second < other.position.second - other.half_size.second);
    }

    void collide(obj &other)
    {
        if(!isColliding(other)) return;

        double l = UINT32_MAX, r = UINT32_MAX, u = UINT32_MAX, d = UINT32_MAX;

        if(position.second - half_size.second < other.position.second + other.half_size.second && position.second + half_size.second > other.position.second + other.half_size.second) u = (position.second - half_size.second) - (other.position.second + other.half_size.second);
        if(position.second - half_size.second < other.position.second - other.half_size.second && position.second + half_size.second > other.position.second - other.half_size.second) d = (position.second + half_size.second) - (other.position.second - other.half_size.second);
        if(position.first - half_size.first < other.position.first + other.half_size.first && position.first + half_size.first > other.position.first + other.half_size.first) l = (position.first - half_size.first) - (other.position.first + other.half_size.first);
        if(position.first - half_size.first < other.position.first - other.half_size.first && position.first + half_size.first > other.position.first - other.half_size.first)r = (position.first + half_size.first) - (other.position.first - other.half_size.first);

        double l_abs = l * l;
        double r_abs = r * r;
        double u_abs = u * u;
        double d_abs = d * d;

        if(other.isStatic)
        {
            if(u_abs < d_abs && u_abs < l_abs && u_abs < r_abs)
            {
                position.second -= u - 0.0001;
                velocity.second = 0.0;
                isGrounded = true;
            }

            if(d_abs < u_abs && d_abs < l_abs && d_abs < r_abs)
            {
                position.second -= d + 0.0001;
                velocity.second = 0.0;
            }

            if(l_abs < u_abs && l_abs < d_abs && l_abs < r_abs)
            {
                position.first -= l - 0.0001;
                velocity.first = 0.0;
            }

            if(r_abs < u_abs && r_abs < d_abs && r_abs < l_abs)
            {
                position.first -= r + 0.0001;
                velocity.first = 0.0;
            }
        }
        else
        {
            if(u_abs < d_abs && u_abs < l_abs && u_abs < r_abs)
            {
                position.second -= (u - 0.0001) * 0.5;
                other.position.second += (u - 0.0001) * 0.5;
                velocity.second = 0.0;
                other.velocity.second = 0.0;
                isGrounded = true;
            }

            if(d_abs < u_abs && d_abs < l_abs && d_abs < r_abs)
            {
                position.second -= (d + 0.0001) * 0.5;
                other.position.second += (d + 0.0001) * 0.5;
                // velocity.second = 0.0;
                // other.velocity.second = 0.0;
            }

            if(l_abs < u_abs && l_abs < d_abs && l_abs < r_abs)
            {
                position.first -= (l - 0.0001) * 0.5;
                other.position.first += (l - 0.0001) * 0.5;
                // velocity.first = 0.0;
                // other.velocity.first = 0.0;
            }

            if(r_abs < u_abs && r_abs < d_abs && r_abs < l_abs)
            {
                position.first -= (r + 0.0001) * 0.5;
                other.position.first += (r + 0.0001) * 0.5;
                // velocity.first = 0.0;
                // other.velocity.first = 0.0;
            }
        }


        // if(position.second - half_size.second < other.position.second + other.half_size.second || position.second + half_size.second > other.position.second - other.half_size.second)
        // {
        //     if(position.second - half_size.second < other.position.second + other.half_size.second && position.second + half_size.second > other.position.second + other.half_size.second)
        //     {
        //         double delta_y = ;
        //         position.second -= delta_y;
        //         velocity.second = 0.0;
        //         isGrounded = true;
        //         // return;
        //     }

        //     if(position.second - half_size.second < other.position.second - other.half_size.second && position.second + half_size.second > other.position.second - other.half_size.second)
        //     {
        //         double delta_y = 
        //         position.second -= delta_y;
        //         velocity.second = 0.0;
        //         // return;
        //     }
        // }

        // if(position.first - half_size.first < other.position.first + other.half_size.first || position.first + half_size.first > other.position.first - other.half_size.first)
        // {
        //     if(position.first - half_size.first < other.position.first + other.half_size.first && position.first + half_size.first > other.position.first + other.half_size.first)
        //     {
        //         double delta_x = 
        //         position.first -= delta_x;
        //         velocity.first = 0.0;
        //         // return;
        //     }

        //     if(position.first - half_size.first < other.position.first - other.half_size.first && position.first + half_size.first > other.position.first - other.half_size.first)
        //     {
        //         double delta_x = 
        //         position.first -= delta_x;
        //         velocity.first = 0.0;
        //         // return;
        //     }
        // }
    }
};

int main()
{
    sf::RenderWindow* window = new sf::RenderWindow();
    int winwidth = sf::VideoMode::getDesktopMode().width;
    int winheight = sf::VideoMode::getDesktopMode().height;
    winwidth = 1280;
    winheight = 720;
    window->create({winwidth, winheight}, "Tallest Towers", sf::Style::Default);
    sf::Clock clock;
    std::vector<obj*> objects;
    double scrollSpeed = 10.0;
    double worldToScreenRatio = 100.0;
    double widthRatio = (double)winwidth / (double)winheight;
    double heightRatio = -1.0;
    const double movementSpeed = 500.0;
    const double jumpMomentum = 20.0;
    const double air_resistance = 0.1;
    const double friction = 50;
    const double slide_friction = 2.5;
    const double g = 9.81;
    double show_fps_timer = 0;
    double dt = 1;
    bool sliding = false;
    std::pair<double, double> mousePos = {0, 0};
    std::pair<double, double> mousePosCentred = {0, 0};

    obj player;
    player.position = {0.0, 5.0};
    player.half_size = {0.5, 0.5};
    player.colour = sf::Color::Red.toInteger();
    player.isStatic = false;
    objects.push_back(&player);

    // obj tmp;
    // tmp.position = {-5.0, 0.0};
    // tmp.colour;
    // objects.push_back(&tmp);

    // obj tmp2;
    // tmp2.position = {5.0, 0.0};
    // tmp2.colour;
    // objects.push_back(&tmp2);

    // obj tmp3;
    // tmp3.position = {3.0, 5.0};
    // tmp3.colour;
    // tmp3.isStatic = false;
    // objects.push_back(&tmp3);

    obj ground;
    ground.position = {0, -5.5};
    ground.half_size = {100, 5.0};
    ground.colour = sf::Color::Green.toInteger();
    objects.push_back(&ground);

    // for(int x = -3; x < 3; x++)
    // {
    //     for(int y = -3; y < 3; y++)
    //     {
    //         obj* tmp4 = new obj();
    //         tmp4->position = {0.0 + (x * 0.1), 50.0 + (y * 2)};
    //         tmp4->half_size = {0.5, 0.5};
    //         tmp4->colour = sf::Color::Cyan.toInteger();
    //         tmp4->isStatic = false;
    //         objects.push_back(tmp4);
    //     }
    // }
    // for(int i = 0; i < 15; i++)
    // {
    //     obj* tmp4 = new obj();
    //     tmp4->position = {0.0, 500.0 + (i * 2)};
    //     tmp4->half_size = {3, 0.5};
    //     tmp4->colour = sf::Color::Cyan.toInteger();
    //     tmp4->isStatic = false;
    //     objects.push_back(tmp4);
    // }

    bool w = false, a = false, s = false, d = false, space = false;

    sf::View camera;
    std::pair<double, double> camPos = {0, 0};
    bool mouse_down = false;

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

            if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    if(!mouse_down)
                    {
                        mouse_down = true;
                        obj* object = new obj();
                        object->position = {(mousePosCentred.first / worldToScreenRatio) + player.position.first, (mousePosCentred.second / worldToScreenRatio) + player.position.second};
                        object->colour = sf::Color::Blue.toInteger();
                        object->isStatic = false;
                        objects.push_back(object);
                    }
                }
            }

            if(event.type == sf::Event::MouseButtonReleased)
            {
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    mouse_down = false;
                }
            }

            if(event.type == sf::Event::KeyPressed)
            {
                if(event.key.code == sf::Keyboard::W)
                {
                    w = true;
                }
                if(event.key.code == sf::Keyboard::S)
                {
                    s = true;
                }
                if(event.key.code == sf::Keyboard::A)
                {
                    a = true;
                }
                if(event.key.code == sf::Keyboard::D)
                {
                    d = true;
                }
                if(event.key.code == sf::Keyboard::Space)
                {
                    space = true;
                }
                if(event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift)
                {
                    sliding = true;
                }
            }

            if(event.type == sf::Event::KeyReleased)
            {
                if(event.key.code == sf::Keyboard::W)
                {
                    w = false;
                }
                if(event.key.code == sf::Keyboard::S)
                {
                    s = false;
                }
                if(event.key.code == sf::Keyboard::A)
                {
                    a = false;
                }
                if(event.key.code == sf::Keyboard::D)
                {
                    d = false;
                }
                if(event.key.code == sf::Keyboard::Space)
                {
                    space = false;
                }
                if(event.key.code == sf::Keyboard::LShift || event.key.code == sf::Keyboard::RShift)
                {
                    sliding = false;
                }
            }

            if(event.type == sf::Event::MouseWheelScrolled)
            {
                worldToScreenRatio += event.mouseWheelScroll.delta * scrollSpeed;
            }

            if(event.type == sf::Event::MouseMoved)
            {
                mousePos.first = event.mouseMove.x;
                mousePos.second = event.mouseMove.y;

                mousePosCentred.first = mousePos.first - (winwidth * 0.5);
                mousePosCentred.second = -(mousePos.second - (winheight * 0.5));
            }
        }

        worldToScreenRatio = std::max(35.0, std::min(200.0, worldToScreenRatio));
        
        if(player.isGrounded && !sliding)
        {
            // player.velocity.second -= w * movementSpeed * dt;
            // player.velocity.second += s * movementSpeed * dt;
            player.velocity.first -= a * movementSpeed * dt;
            player.velocity.first += d * movementSpeed * dt;

            if(space) player.velocity.second = jumpMomentum;
        }

        camPos = lerp(player.position, mousePosCentred, 0.0005);
        camera.setCenter({camPos.first * worldToScreenRatio / widthRatio, camPos.second * worldToScreenRatio * heightRatio});
        window->setView(camera);
        
        window->clear();

        // if(tmp.isColliding(player)) tmp.colour = sf::Color::Yellow.toInteger();
        // else tmp.colour = sf::Color::Blue.toInteger();
        // if(tmp2.isColliding(player)) tmp2.colour = sf::Color::Yellow.toInteger();
        // else tmp2.colour = sf::Color::Blue.toInteger();
        
        for(int i = 0; i < objects.size(); i++)
        {
            if(!objects[i]->isStatic)
            {
                if(objects[i] == &player && sliding)
                {
                    if(objects[i]->isGrounded)
                    {
                        objects[i]->velocity.first -= (objects[i]->velocity.first * slide_friction * dt);
                        objects[i]->velocity.second -= (objects[i]->velocity.second * slide_friction * dt);
                    }
                    else
                    {
                        objects[i]->velocity.first -= (objects[i]->velocity.first * air_resistance * dt);
                        objects[i]->velocity.second -= (objects[i]->velocity.second * air_resistance * dt);
                        objects[i]->velocity.second -= g * dt * 5;
                    }
                }
                else
                {
                    if(objects[i]->isGrounded)
                    {
                        objects[i]->velocity.first -= (objects[i]->velocity.first * friction * dt);
                        objects[i]->velocity.second -= (objects[i]->velocity.second * friction * dt);
                    }
                    else
                    {
                        objects[i]->velocity.first -= (objects[i]->velocity.first * air_resistance * dt);
                        objects[i]->velocity.second -= (objects[i]->velocity.second * air_resistance * dt);
                        objects[i]->velocity.second -= g * dt * 5;
                    }
                }
                objects[i]->isGrounded = false;
                for(auto object : objects)
                {
                    if(object == objects[i]) continue;
                    objects[i]->collide(*object);
                }
                objects[i]->position.first += objects[i]->velocity.first * dt;
                objects[i]->position.second += objects[i]->velocity.second * dt;
            }

            sf::RectangleShape rect;
            rect.setPosition({objects[i]->position.first * worldToScreenRatio / widthRatio, objects[i]->position.second * worldToScreenRatio * heightRatio});
            rect.setSize({objects[i]->half_size.first * 2 * worldToScreenRatio / widthRatio, objects[i]->half_size.second * 2 * worldToScreenRatio * heightRatio});
            rect.setOrigin({objects[i]->half_size.first * worldToScreenRatio / widthRatio, objects[i]->half_size.second * worldToScreenRatio * heightRatio});
            rect.setFillColor(sf::Color(objects[i]->colour));
            rect.setOutlineColor(sf::Color::Black);
            rect.setOutlineThickness(1);
            window->draw(rect);
        }

        window->display();

        dt = clock.restart().asSeconds();
        show_fps_timer += dt;
        if(show_fps_timer >= 0.5)
        {
            show_fps_timer -= 0.5;
            window->setTitle("Tallest Towers - " + std::to_string((int)(1.0 / dt)) + " FPS");
        }
    }

    delete window;
    
    return 0;
}
