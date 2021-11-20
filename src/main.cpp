#include <unistd.h> // THis must be included before 'e131.h'.
#include "e131.h"

#include <cstdio>
#include <cstdint>

int main(int argc, char* argv[])
{
    static const uint16_t PORT = 8888;
    static const uint16_t UNIVERSE_1 = 1;
    static const uint16_t UNIVERSE_2 = 2;

    int sockfd = e131_socket();
    if (sockfd < 0)
    {
        fprintf(stderr, "Couldn't create e131 socket.\n");
        return 1;
    }

    e131_addr_t dest_universe_1;
    if (e131_multicast_dest(&dest_universe_1, UNIVERSE_1, PORT) < 0)
    {
        fprintf(stderr, "Couldn't create a multicast destination on universe %d port %d\n", UNIVERSE_1, PORT);
        return 1;
    }

    e131_packet_t packet;
    e131_pkt_init(&packet, UNIVERSE_1, 24);

    return 0;
}
