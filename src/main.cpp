#include "cube.h"
#include "remote.h"
#include "episodes.h"

#include <unistd.h> // This must be included before 'e131.h'.
#include "e131.h"

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <ctime>

#include "game.h"

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

    GameState* game = game::create_state();

    game::start(game);

    episodes::start_episode(cube, remote_system, Episode::FUN_01);

    cube::destroy(cube);
    remote::destroy_system(remote_system);
    return 0;
}
