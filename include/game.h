#pragma once

#include <vector>
#include "cube.h"

class RemoteSystem;
class Cube;

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
    void play_turn(GameState&, RemoteSystem*);

    void draw(GameState&, Cube*);
} // namespace game
