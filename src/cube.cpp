#include <iostream>
#include <unistd.h> // This must be included before 'e131.h'.
#include "e131.h"
#include "cube.h"

#include <vector>
#include <cstring>

using namespace std;

struct Tal
{
    // Vec3 pos;
    int index;
    Color leds[3];
};

struct UniverseConnexion
{
    unsigned int universeId;
    unsigned int universeSize;
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
    const uint32_t mapping[4][4][4] = {
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
        if (e131_pkt_validate(&cnx.packet) != E131_ERR_NONE
            || e131_send(cnx.sockfd, &cnx.packet, &cnx.dest) < 0)
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

    void applyColor(UniverseConnexion &cnx, unsigned int index, Tal tal)
    {
        cnx.packet.dmp.prop_val[index] = tal.leds[0].r;
        cnx.packet.dmp.prop_val[index + 1] = tal.leds[0].g;
        cnx.packet.dmp.prop_val[index + 2] = tal.leds[0].b;
    }

} // anonymous namespace

namespace cube
{
    Cube *create()
    {
        Cube *c = new Cube();
        // c->universe1 = new UniverseConnexion();
        // c->universe2 = new UniverseConnexion();

        c->universe1.universeId = 1;
        c->universe2.universeId = 2;

        c->universe1.universeSize = 510;
        c->universe2.universeSize = 66;

        c->tals = std::vector<Tal>();

        for (int cpt = 0; cpt < 64; ++cpt)
        {
            c->tals.push_back({});
        }

        return c;
    }

    void destroy(Cube *cube)
    {
        delete cube;
    }

    bool initUniverse(UniverseConnexion &cnx)
    {
        // std::cout << cnx.universeId << std::endl;
        cnx.sockfd = e131_socket();
        if (cnx.sockfd < 0)
        {
            fprintf(stderr, "Couldn't create e131 socket.\n");
            return false;
        }
        // std::cout << cnx->sockfd << std::endl;

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

    unsigned int convertVec3ToIndex(const Vec3 &pos)
    {
        return mapping[pos.z][pos.x][pos.y];
    }

    void ligthTal(Cube *cube, Vec3 tal, Color color)
    {
        for (unsigned int i = 0; i < 3; i++)
        {
            cube::ligthLed(cube, tal, i, color);
        }
    }

    void ligthTal(Cube *cube, unsigned int talIdex, Color color)
    {
        for (unsigned int i = 0; i < 3; i++)
        {
            cube::ligthLed(cube, talIdex, i, color);
        }
    }

    void ligthLed(Cube *cube, Vec3 talPos, unsigned int ledIndex, Color color)
    {
        unsigned int talIndex = convertVec3ToIndex(talPos);
        talIndex = (talIndex - 1) * 3 + 1;
        ligthLed(cube, talIndex, ledIndex, color);
    }

    void ligthLed(Cube *cube, unsigned int talIdex, unsigned int led, Color color)
    {
        cube->tals[talIdex].leds[led].r = color.r;
        cube->tals[talIdex].leds[led].g = color.g;
        cube->tals[talIdex].leds[led].b = color.b;
    }

    void commit(Cube *cube)
    {
        for (auto tal : cube->tals)
        {
            unsigned int index = tal.index * 3;
            // int index = convrtVec3ToIndex(tal);

            if (index > cube->universe1.universeSize)
            {
                applyColor(cube->universe1, index, tal);
                applyColor(cube->universe1, index + 1, tal);
            }
            else
            {
                applyColor(cube->universe2, index % cube->universe1.universeSize, tal);
                applyColor(cube->universe2, (index + 1) % cube->universe1.universeSize, tal);
            }

            if (index > cube->universe1.universeSize)
            {
                applyColor(cube->universe1, index + 2, tal);
            }
            else
            {
                applyColor(cube->universe2, (index + 2) % cube->universe1.universeSize, tal);
            }
        }

        send(cube);
        resetTals(cube);
    }
} // namespace cube
