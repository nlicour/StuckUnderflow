#pragma once

#include <vector>
#include "cube.h"

struct Player
{
    int id;
    std::vector<Tal> tals;
    Color cursorColor;
    Color dotColor;
};

struct GameState
{
    Tal currentTal;
    Player player1;
    Player player2;
};


namespace game
{
    GameState *create_state();
    void destroy_state(GameState *);

    void start(GameState* gameState);

} // namespace game
