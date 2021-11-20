#include <unistd.h> // This must be included before 'e131.h'.
#include "e131.h"
#include "cube.h"

#include <vector>
#include <cstring>

using namespace std;

struct Tal
{
    Vec3 pos;
    Color leds[3];
};

struct Cube
{
    std::vector<Tal> tals;
    int sockfd_u1; // Socket for universe 1.
    int sockfd_u2; // Socket for universe 2.

    e131_addr_t dest_u1; // Destination address for universe 1.
    e131_addr_t dest_u2; // Destination address for universe 2.
};

namespace cube
{
    Cube* create()
    {
        return new Cube();
    }

    void destroy(Cube* cube)
    {
        delete cube;
    }

    bool init(Cube* cube)
    {
        cube->sockfd_u1 = e131_socket();
        cube->sockfd_u2 = e131_socket();

        if (cube->sockfd_u1 < 0 || cube->sockfd_u2 < 0)
        {
            fprintf(stderr, "Couldn't create e131 socket.\n");
            return false;
        }

        if (e131_multicast_dest(&cube->dest_u1, 1, E131_DEFAULT_PORT) < 0
            || e131_multicast_dest(&cube->dest_u2, 2, E131_DEFAULT_PORT) < 0)
        {
            fprintf(stderr, "Couldn't create a multicast destinations\n");
            return false;
        }

        if (e131_multicast_join(cube->sockfd_u1, 1) < 0
            || e131_multicast_join(cube->sockfd_u2, 2) < 0)
        {
            fprintf(stderr, "Couldn't join sockets to universes\n");
            return false;
        }

        return true;
    }
    void ligthTal(Cube* cube, Vec3 tal, Color color)
    {
        for (unsigned int i = 0; i < 3; i++)
        {
            cube::ligthLed(cube, tal, i, color);
        }
    }

    void ligthLed(Cube* cube, Vec3 talPos, unsigned int led, Color color)
    {
        for (Tal tmpTal : cube->tals)
        {
            if (tmpTal.pos == talPos)
            {
                tmpTal.leds[led].r = color.r;
                tmpTal.leds[led].g = color.g;
                tmpTal.leds[led].b = color.b;
            }
        }
    }
} // namespace cube
