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
    Vec3 currentPlayerPos;
    Tal currentTal;
    Player player1;
    Player player2;
    Cube* cube;
};

namespace game
{
    GameState *create_state();
    void destroy_state(GameState *);

    void start(GameState* gameState);

    void move(GameState* gameState, Vec3 move);
} // namespace game
