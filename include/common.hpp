#pragma once

#include <chrono>
#include <unordered_map>

#include <network.hpp>
#include <utils.hpp>
#include <version.hpp>

class entity
{
public:
    entity()
    {
        id = std::chrono::system_clock::now().time_since_epoch().count();
    }

    size_t id;
    ut::Vector2d position;
    ut::Vector2d velocity;
    ut::Vector2d size;
    size_t sprite;
    int owner = -1;
};

class tile
{
public:
    tile()
    {

    }

    ut::Vector2i position;
    ut::Vector2i size;
    uint8_t layer;
    bool collides = false;
};

enum message_types
{
    CHAT_MSG, UPDATE_ENTITIES, ADD_ENTITIES
};