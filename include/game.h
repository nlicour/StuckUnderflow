#pragma once 

#include <vector>
#include "cube.h"

struct GameState
{
    Tal currentTal;
    Player player1;
    Player player2;
};

struct Player
{
    int id;
    std::vector<Tal> tals;
    Color cursorColor;
    Color dotColor;
};