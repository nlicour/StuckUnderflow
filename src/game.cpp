#include "game.h"
#include "cube.h"

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

    void start(GameState *gameState)
    {
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

    void draw(GameState& gs, Cube* cube)
    {
    }
} // namespace game
