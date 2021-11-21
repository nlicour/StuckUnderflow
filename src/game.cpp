#include "game.h"


namespace remote
{
    GameState *create_system()
    {
        return new GameState();
    }

    void destroy_system(GameState *rs)
    {
        delete rs;
    }

    void start(GameState* gameState){
        
    }
}