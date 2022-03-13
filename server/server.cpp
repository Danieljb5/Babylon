#include <common.hpp>

#define SERVER
#ifndef DIST
    #include <debug.hpp>
#endif

std::unordered_map<uint64_t, entity> entities;
std::vector<tile*> all_tiles;
ut::StaticQuadTree<tile*> collision_tiles;

class Server : public net::server_interface<message_types>
{
public:
    Server(uint16_t port) : net::server_interface<message_types>(port, global_version) {}

protected:
    bool onClientConnect(std::shared_ptr<net::connection<message_types>> client) override
    {
        return true;
    }

    void onMessage(std::shared_ptr<net::connection<message_types>> client, net::message<message_types> &msg) override
    {
        messageAllClients(msg, client);
    }
};

int main()
{
#ifndef DIST
    test_debug_version(global_version);
#endif

    Server server(36000);
    server.start();

    while(1)
    {
        server.update(-1, true);
    }
}