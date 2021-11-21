#include "cube.h"
#include "remote.h"

#include <unistd.h> // This must be included before 'e131.h'.
#include "e131.h"

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[])
{
    srand(time(NULL));

    RemoteSystem* remote_system = remote::create_system();
    remote::connect(remote_system, 0);
    remote::connect(remote_system, 1);

    Cube* cube = cube::create();
    if (!cube::init(cube))
    {
        fprintf(stderr, "Couldn't initialize a cube\n");
        return -1;
    }

    Vec3 pos = {0, 0, 0};
    for (;;)
    {
        uint16_t buttons = remote::wait_for_state_change(remote_system, 0);

        if (buttons & 0x1) pos.x = (pos.x + 1) % 4;
        if (buttons & 0x2) pos.y = (pos.y + 1) % 4;
        if (buttons & 0x4) pos.z = (pos.z + 1) % 4;

        cube::lightTal(cube, pos, {255, 0, 255});
        cube::commit(cube);

        usleep(25000);
    }

    cube::destroy(cube);
    remote::destroy_system(remote_system);
    return 0;
}
