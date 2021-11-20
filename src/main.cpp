#include "cube.h"
#include "remote.h"
#include "episodes.h"

#include <unistd.h> // This must be included before 'e131.h'.
#include "e131.h"

#include <cstdio>
#include <cstdint>
#include <cstring>

int main(int argc, char* argv[])
{
    RemoteSystem* remote_system = remote::create_system();

    if (!remote::connect(remote_system, 0))
    {
        fprintf(stderr, "Couldn't connect to remote %d\n", 0);
    }

    if (!remote::connect(remote_system, 1))
    {
        fprintf(stderr, "Couldn't connect to remote %d\n", 1);
    }

    Cube* cube = cube::create();
    if (!cube::init(cube))
    {
        fprintf(stderr, "Couldn't initialize a cube\n");
        return -1;
    }

    uint16_t buttons_1;
    uint16_t buttons_2;
    for(;;)
    {
        bool remote_1_has_event = false;
        bool remote_2_has_event = false;

        while (remote::poll_remote(remote_system, 0, &buttons_1))
        {
            remote_1_has_event = true;
        }

        while (remote::poll_remote(remote_system, 1, &buttons_2))
        {
            remote_2_has_event = true;
        }

        if (remote_1_has_event)
        {
            printf("Remote 1 received: %d\n", buttons_1);
        }
        if (remote_2_has_event)
        {
            printf("Remote 2 received: %d\n", buttons_2);
        }
    }

    // episodes::start_episode(cube, Episode::S01E03);

    cube::destroy(cube);
    remote::destroy_system(remote_system);
    return 0;
}
