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
    else
    {
        printf("Remote connected.\n");
    }

    Cube* cube = cube::create();
    if (!cube::init(cube))
    {
        fprintf(stderr, "Couldn't initialize a cube\n");
        return -1;
    }

    // uint16_t buttons;
    // for(;;)
    // {
    //     while (remote::poll_remote(remote_system, 0, &buttons));
    // }

    episodes::start_episode(cube, remote_system, Episode::S02E01);

    cube::destroy(cube);
    remote::destroy_system(remote_system);
    return 0;
}
