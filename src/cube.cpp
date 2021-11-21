#include <iostream>
#include <unistd.h> // This must be included before 'e131.h'.
#include "e131.h"
#include "cube.h"

#include <vector>
#include <cstring>

using namespace std;

struct UniverseConnexion
{
    uint16_t universeId;
    uint16_t universeSize;
    int sockfd;       // Socket
    e131_addr_t dest; // Destination
    e131_packet_t packet;
};

struct Cube
{
    std::vector<Tal> tals;
    UniverseConnexion universe1;
    UniverseConnexion universe2;
};

namespace
{
    const uint32_t mapping[4][4][4] = 
    {
        {
            {1, 35, 38, 40},
            {15, 25, 33, 32},
            {18, 23, 28, 31},
            {20, 21, 22, 30},
        },
        {
            {55, 45, 43, 41},
            {5, 2, 36, 39},
            {13, 16, 26, 34},
            {12, 19, 24, 29},
        },
        {
            {58, 53, 48, 42},
            {63, 56, 46, 44},
            {8, 6, 3, 37},
            {11, 14, 17, 27},
        },
        {
            {60, 52, 51, 50},
            {61, 59, 54, 49},
            {62, 64, 57, 47},
            {10, 9, 7, 4},
        },
    };

    void send(const UniverseConnexion &cnx)
    {
        if (e131_pkt_validate(&cnx.packet) != E131_ERR_NONE || e131_send(cnx.sockfd, &cnx.packet, &cnx.dest) < 0)
        {
            fprintf(stderr, "Packet isn't well formed or couldn't be sent.\n");
            e131_pkt_dump(stderr, &cnx.packet);
            return;
        }
    }

    void send(Cube *cube)
    {
        send(cube->universe1);
        send(cube->universe2);
    }

    void resetTals(Cube *cube)
    {
        for (auto &tal : cube->tals)
        {
            tal.leds[0].r = 0;
            tal.leds[0].g = 0;
            tal.leds[0].b = 0;

            tal.leds[1].r = 0;
            tal.leds[1].g = 0;
            tal.leds[1].b = 0;

            tal.leds[2].r = 0;
            tal.leds[2].g = 0;
            tal.leds[2].b = 0;
        }
    }

    void applyColor(Cube *cube, uint8_t tal_index, uint8_t led_index, Tal tal)
    {
        uint16_t slot = tal_index * 9 + led_index * 3 + 1;

        if (slot > cube->universe1.universeSize)
        {
            slot -= 510;

            auto &packet = cube->universe2.packet;
            packet.dmp.prop_val[slot + 0] = tal.leds[led_index].r;
            packet.dmp.prop_val[slot + 1] = tal.leds[led_index].g;
            packet.dmp.prop_val[slot + 2] = tal.leds[led_index].b;
        }
        else
        {
            auto &packet = cube->universe1.packet;
            packet.dmp.prop_val[slot + 0] = tal.leds[led_index].r;
            packet.dmp.prop_val[slot + 1] = tal.leds[led_index].g;
            packet.dmp.prop_val[slot + 2] = tal.leds[led_index].b;
        }
    }

} // anonymous namespace

namespace cube
{
    Cube *create()
    {
        Cube *c = new Cube();

        c->universe1.universeId = 1;
        c->universe2.universeId = 2;

        c->universe1.universeSize = 510;
        c->universe2.universeSize = 66;

        c->tals.resize(64);

        return c;
    }

    void destroy(Cube *cube)
    {
        delete cube;
    }

    bool initUniverse(UniverseConnexion &cnx)
    {
        cnx.sockfd = e131_socket();
        if (cnx.sockfd < 0)
        {
            fprintf(stderr, "Couldn't create e131 socket.\n");
            return false;
        }

        if (e131_multicast_dest(&cnx.dest, cnx.universeId, E131_DEFAULT_PORT) < 0)
        {
            fprintf(stderr, "Couldn't create a multicast destinations\n");
            return false;
        }

        if (e131_multicast_join(cnx.sockfd, cnx.universeId) < 0)
        {
            fprintf(stderr, "Couldn't join sockets to universes\n");
            return false;
        }

        e131_pkt_init(&cnx.packet, cnx.universeId, cnx.universeSize);

        if (e131_set_option(&cnx.packet, E131_OPT_PREVIEW, true) < 0)
        {
            fprintf(stderr, "Couldn't set e131 option.\n");
            return false;
        }

        memcpy(cnx.packet.frame.source_name, "Stuckunderflower", 16);

        return true;
    }

    bool init(Cube *cube)
    {
        return initUniverse(cube->universe1) && initUniverse(cube->universe2);
    }

    uint8_t vec3_to_tal_index(const Vec3 &pos)
    {
        return mapping[pos.z][pos.x][pos.y];
    }

    bool isVecValid(Vec3 &tal)
    {
        return tal.x <= 3 && tal.y <= 3 && tal.z <= 3;
    }

    void lightTal(Cube *cube, Vec3 tal, Color color)
    {
        if (!isVecValid(tal))
        {
            std::cout << "[ERROR] invalid input" << std::endl;
            return;
        }
        for (uint8_t i = 0; i < 3; i++)
        {
            cube::lightLed(cube, tal, i, color);
        }
    }

    void lightLed(Cube *cube, Vec3 talPos, uint8_t led_index, Color color)
    {
        uint8_t tal_index = vec3_to_tal_index(talPos);

        cube->tals[tal_index - 1].leds[led_index].r = color.r;
        cube->tals[tal_index - 1].leds[led_index].g = color.g;
        cube->tals[tal_index - 1].leds[led_index].b = color.b;
    }

    void commit(Cube *cube)
    {
        for (uint8_t tal_index = 0; tal_index < cube->tals.size(); ++tal_index)
        {
            for (uint8_t led_index = 0; led_index < 3; ++led_index)
            {
                applyColor(cube, tal_index, led_index, cube->tals[tal_index]);
            }
        }

        send(cube);
        resetTals(cube);
    }
} // namespace cube
