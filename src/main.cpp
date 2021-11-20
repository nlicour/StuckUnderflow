#include "cube.h"

#include <unistd.h> // This must be included before 'e131.h'.
#include "e131.h"

#include <cstdio>
#include <cstdint>
#include <cstring>

int main(int argc, char* argv[])
{
    /* For reference, delete once cube.cpp will be complete.
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
    */

    Cube* cube = cube::create();
    if (!cube::init(cube))
    {
        fprintf(stderr, "Couldn't initialize a cube\n");
        return -1;
    }

    for(;;)
    {
        Color x;
        x.r = 255;
        x.g = 255;
        x.b = 255;

        cube::ligthTal(cube, 2, x);
    }

    cube::destroy(cube);

    /* For reference, delete once cube.cpp will be complete.
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
    */

    return 0;
}
