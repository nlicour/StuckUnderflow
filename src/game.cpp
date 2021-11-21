#include "game.h"
#include "cube.h"

#include <cstdio>
#include <unistd.h>

#define MS_TO_US(ms) (ms * 1000)

namespace
{
void clear_cube(Cube* cube)
{
    for (uint8_t x = 0; x < 4; ++x)
    {
        for (uint8_t y = 0; y < 4; ++y)
        {
            for (uint8_t z = 0; z < 4; ++z)
            {
                cube::lightTal(cube, {x, y, z}, {0, 0, 0});
            }
        }
    }

    cube::commit(cube);
}

void do_start_animation(GameState* gs, Cube* cube)
{
    clear_cube(cube);

    for (uint8_t x = 0; x < 4; ++x)
    {
        if (x == 1 || x == 2) continue;

        for (uint8_t y = 0; y < 4; ++y)
        {
            if (y == 1 || y == 2) continue;

            for (uint8_t z = 0; z < 4; ++z)
            {
                if (z == 1 || z == 2) continue;

                cube::lightTal(cube, {x, y, z}, {255, 0, 0});
            }
        }
    }
    cube::commit(cube);
    usleep(MS_TO_US(5000));
}
} // anonymous namespace

void updateGrid(GameState* gameState){

    // gameState->cube

}

namespace game
{
    GameState *create_state()
    {
        return new GameState();
    }

    void destroy_state(GameState *rs)
    {
        delete rs;
    }

    void reset(GameState* gs)
    {
        gs->run_start_animation = true;
    }

    void move(GameState *gameState, Vec3 move)
    {
        gameState->currentPlayerPos.x = (gameState->currentPlayerPos.x + (move.x % 2)) % 4;
        gameState->currentPlayerPos.y = (gameState->currentPlayerPos.y + (move.y % 2)) % 4;
        gameState->currentPlayerPos.z = (gameState->currentPlayerPos.z + (move.z % 2)) % 4;


    }

    void play_turn(GameState& gs, RemoteSystem* rs)
    {
    }

    void draw(GameState* gs, Cube* cube)
    {
        if (gs->run_start_animation)
        {
            do_start_animation(gs, cube);
            gs->run_start_animation = false;
        }
    }
} // namespace game
