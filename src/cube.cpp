#include "cube.h"

#include <unistd.h> // This must be included before 'e131.h'.
#include "e131.h"

#include <cstring>

struct Cube
{
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
} // namespace cube
