#include "game.h"

namespace game
{
    GameState *create_state()
    {
        return new GameState();
    }

    void destroy_system(GameState *rs)
    {
        delete rs;
    }

    void start(GameState* gameState){
        
    }

    void play_turn(GameState& gs, RemoteSystem* rs)
    {
    }

    void draw(GameState& gs, Cube* cube)
    {
    }
} // namespace game
