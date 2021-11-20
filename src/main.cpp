#include <unistd.h> // This must be included before 'e131.h'.
#include "e131.h"

#include <cstdio>
#include <cstdint>
#include <cstring>

int main(int argc, char* argv[])
{
    static const uint16_t UNIVERSE_1 = 1;
    static const uint16_t UNIVERSE_2 = 2;

    int sockfd_u1 = e131_socket();
    int sockfd_u2 = e131_socket();

    e131_addr_t dest_u1;
    e131_addr_t dest_u2;

    e131_multicast_dest(&dest_u1, 1, E131_DEFAULT_PORT);
    e131_multicast_dest(&dest_u2, 2, E131_DEFAULT_PORT);

    e131_multicast_join(sockfd_u1, 1);
    e131_multicast_join(sockfd_u2, 2);

    // Configure packet.
    e131_packet_t packet_u1;
    e131_pkt_init(&packet_u1, 1, 510);
    memcpy(packet_u1.frame.source_name, "Stuckunderflower", 16);
    e131_set_option(&packet_u1, E131_OPT_PREVIEW, true);
    packet_u1.frame.seq_number = 0;

    e131_packet_t packet_u2;
    e131_pkt_init(&packet_u2, 1, 66);
    memcpy(packet_u2.frame.source_name, "Stuckunderflower", 16);
    e131_set_option(&packet_u2, E131_OPT_PREVIEW, true);
    packet_u2.frame.seq_number = 1;

    // Clear universes.
    e131_send(sockfd_u1, &packet_u1, &dest_u1);
    e131_send(sockfd_u2, &packet_u2, &dest_u2);

    packet_u1.frame.seq_number = 2;

    /*
    // 1 - 36 axe vertical.
    //      1 - 9 -> 1TAL
    //      10 - 18 -> 2TAL
    uint32_t slot_start = 1;
    uint32_t slot_end = slot_start + 9;
    while (slot_end <= 510)
    {
        for (uint32_t i = slot_start; i <= slot_end; i+=3)
        {
            packet_u1.dmp.prop_val[i + 0] = 255;
            packet_u1.dmp.prop_val[i + 1] = 255;
            packet_u1.dmp.prop_val[i + 2] = 255;
        }

        if (e131_send(sockfd_u1, &packet_u1, &dest_u1) < 0)
        {
            fprintf(stderr, "Couldn't send packet_u1.\n");
            e131_pkt_dump(stderr, &packet_u1);
            return -1;
        }
        packet_u1.frame.seq_number++;
        usleep(500000);

        slot_end += 9;
    }
    */

    return 0;

    /*
    0
        0
    0       0
        0       0
    0       0
        0
    0
    */

    /*
    packet.dmp.prop_val[1] = 255;
    packet.dmp.prop_val[2] = 255;
    packet.dmp.prop_val[3] = 255;

    packet.dmp.prop_val[7] = 255;
    packet.dmp.prop_val[8] = 255;
    packet.dmp.prop_val[9] = 255;

    packet.dmp.prop_val[10] = 255;
    packet.dmp.prop_val[11] = 255;
    packet.dmp.prop_val[12] = 255;

    packet.dmp.prop_val[13] = 255;
    packet.dmp.prop_val[14] = 255;
    packet.dmp.prop_val[15] = 255;
    */

    /*
    if (e131_pkt_validate(&packet) != E131_ERR_NONE)
    {
        fprintf(stderr, "Packet isn't well-formed.\n");
        e131_pkt_dump(stdout, &packet);
        return -1;
    }

    for(;;)
    {
        if (e131_send(sockfd_u1, &packet, &dest_u1) < 0)
        {
            fprintf(stderr, "Couldn't send packet.\n");
            e131_pkt_dump(stderr, &packet);
            return -1;
        }
        packet.frame.seq_number++;
        usleep(250000);
    }
    */

    return 0;
}
