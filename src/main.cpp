#include "cube.h"

#include <unistd.h> // This must be included before 'e131.h'.
#include "e131.h"

#include <cstdio>
#include <cstdint>
#include <cstring>

void ex1(Cube* cube){

    cube::ligthTal(cube, {0, 0, 0}, {255, 255, 255});
    cube::ligthTal(cube, {1, 0, 0}, {255, 255, 255});
    cube::ligthTal(cube, {2, 0, 0}, {255, 255, 255});
    cube::ligthTal(cube, {3, 0, 0}, {255, 255, 255});

    cube::ligthTal(cube, {0, 1, 0}, {255, 0, 255});
    cube::ligthTal(cube, {1, 1, 0}, {255, 0, 255});
    cube::ligthTal(cube, {2, 1, 0}, {255, 0, 255});
    cube::ligthTal(cube, {3, 1, 0}, {255, 0, 255});


    cube::ligthTal(cube, {0, 2, 0}, {255, 255, 0});
    cube::ligthTal(cube, {1, 2, 0}, {255, 255, 0});
    cube::ligthTal(cube, {2, 2, 0}, {255, 255, 0});
    cube::ligthTal(cube, {3, 2, 0}, {255, 255, 0});


    cube::ligthTal(cube, {0, 3, 0}, {0, 255, 255});
    cube::ligthTal(cube, {1, 3, 0}, {0, 255, 255});
    cube::ligthTal(cube, {2, 3, 0}, {0, 255, 255});
    cube::ligthTal(cube, {3, 3, 0}, {0, 255, 255});


    cube::ligthTal(cube, {0, 0, 3}, {128, 128, 128});
    cube::ligthTal(cube, {1, 0, 3}, {128, 128, 128});
    cube::ligthTal(cube, {2, 0, 3}, {128, 128, 128});
    cube::ligthTal(cube, {3, 0, 3}, {128, 128, 128});
    cube::commit(cube);
}

void ex2(Cube* cube){

    cube::ligthTal(cube, {0, 0, 0}, {0, 0, 0});
    cube::ligthTal(cube, {1, 0, 0}, {0, 0, 0});
    cube::ligthTal(cube, {2, 0, 0}, {0, 0, 0});
    cube::ligthTal(cube, {3, 0, 0}, {0, 0, 0});

    cube::ligthTal(cube, {0, 1, 0}, {0, 0, 0});
    cube::ligthTal(cube, {1, 1, 0}, {0, 0, 0});
    cube::ligthTal(cube, {2, 1, 0}, {0, 0, 0});
    cube::ligthTal(cube, {3, 1, 0}, {0, 0, 0});


    cube::ligthTal(cube, {0, 2, 0}, {0, 0, 0});
    cube::ligthTal(cube, {1, 2, 0}, {0, 0, 0});
    cube::ligthTal(cube, {2, 2, 0}, {0, 0, 0});
    cube::ligthTal(cube, {3, 2, 0}, {0, 0, 0});


    cube::ligthTal(cube, {0, 3, 0}, {0, 0, 0});
    cube::ligthTal(cube, {1, 3, 0}, {0, 0, 0});
    cube::ligthTal(cube, {2, 3, 0}, {0, 0, 0});
    cube::ligthTal(cube, {3, 3, 0}, {0, 0, 0});


    cube::ligthTal(cube, {0, 0, 3}, {0, 0, 0});
    cube::ligthTal(cube, {1, 0, 3}, {0, 0, 0});
    cube::ligthTal(cube, {2, 0, 3}, {0, 0, 0});
    cube::ligthTal(cube, {3, 0, 3}, {0, 0, 0});
    cube::commit(cube);
}

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
        ex1(cube);
        usleep(250000);
        ex2(cube);
        usleep(250000);
    }
    cube::ligthTal(cube, 2, {255, 255, 255});
    cube::commit(cube);

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
