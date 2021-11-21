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
    Tal currentTal;
    Player player1;
    Player player2;
};

namespace game
{
void play_turn(GameState&, RemoteSystem*);

void draw(GameState&, Cube*);
} // namespace game
