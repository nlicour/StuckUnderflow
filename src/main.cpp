#include <unistd.h> // This must be included before 'e131.h'.
#include "e131.h"

#include <cstdio>
#include <cstdint>
#include <cstring>

int main(int argc, char* argv[])
{
    static const uint16_t UNIVERSE_1 = 1;
    static const uint16_t UNIVERSE_2 = 2;

    int sockfd = e131_socket();
    if (sockfd < 0)
    {
        fprintf(stderr, "Couldn't create e131 socket.\n");
        return -1;
    }

    e131_addr_t dest_universe_1;
    if (e131_multicast_dest(&dest_universe_1, UNIVERSE_1, E131_DEFAULT_PORT) < 0)
    {
        fprintf(stderr, "Couldn't create a multicast destination on universe %d port %d\n", UNIVERSE_1, E131_DEFAULT_PORT);
        return -1;
    }

    if (e131_multicast_join(sockfd, UNIVERSE_1) < 0)
    {
        fprintf(stderr, "Couldn't join socket to universe %d\n", UNIVERSE_1);
        return -1;
    }

    // Configure packet.
    e131_packet_t packet;
    e131_pkt_init(&packet, UNIVERSE_1, 510);
    memcpy(packet.frame.source_name, "Stuckunderflower", 16);

    if (e131_set_option(&packet, E131_OPT_PREVIEW, true) < 0)
    {
        fprintf(stderr, "Couldn't set e131 option.\n");
        return false;
    }

    packet.dmp.prop_val[10] = 255;
    packet.dmp.prop_val[11] = 255;
    packet.dmp.prop_val[12] = 255;

    if (e131_pkt_validate(&packet) != E131_ERR_NONE)
    {
        fprintf(stderr, "Packet isn't well-formed.\n");
        e131_pkt_dump(stdout, &packet);
        return -1;
    }

    for(;;)
    {
        if (e131_send(sockfd, &packet, &dest_universe_1) < 0)
        {
            fprintf(stderr, "Couldn't send packet.\n");
            e131_pkt_dump(stderr, &packet);
            return -1;
        }
        packet.frame.seq_number++;
        usleep(250000);
    }

    return 0;
}
