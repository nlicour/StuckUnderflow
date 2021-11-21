#include "cube.h"
#include "remote.h"

#include <unistd.h> // This must be included before 'e131.h'.
#include "e131.h"

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <ctime>

#include "game.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    RemoteSystem *remote_system = remote::create_system();
    remote::connect(remote_system, 0);
    remote::connect(remote_system, 1);

    Cube *cube = cube::create();
    if (!cube::init(cube))
    {
        fprintf(stderr, "Couldn't initialize a cube\n");
        return -1;
    }

    GameState *game = game::create_state();

    // game::start(game);

    Vec3 pos[2] = {{0, 0, 0}, {3, 3, 3}};
    uint8_t remote_id = 0;
    for (;;)
    {
        uint16_t buttons = remote::wait_for_state_change(remote_system, remote_id);

        if (buttons & 0x1)
            pos[remote_id].x = (pos[remote_id].x + 1) % 4;
        if (buttons & 0x2)
            pos[remote_id].y = (pos[remote_id].y + 1) % 4;
        if (buttons & 0x4)
            pos[remote_id].z = (pos[remote_id].z + 1) % 4;

        cube::lightTal(cube, pos[remote_id], {255, 0, 255});
        cube::commit(cube);

        remote_id = (remote_id + 1) % 2;
    }

    cube::destroy(cube);
    remote::destroy_system(remote_system);
    return 0;
}
