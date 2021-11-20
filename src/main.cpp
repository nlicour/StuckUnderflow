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

    Cube* cube = cube::create();
    if (!cube::init(cube))
    {
        fprintf(stderr, "Couldn't initialize a cube\n");
        return -1;
    }

    // episodes::start_episode(cube, remote_system, Episode::S02E01);
    episodes::start_episode(cube, remote_system, Episode::S01E04);

    cube::destroy(cube);
    remote::destroy_system(remote_system);
    return 0;
}
