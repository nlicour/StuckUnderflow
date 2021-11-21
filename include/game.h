#pragma once

#include <vector>
#include "cube.h"

class RemoteSystem;
class Cube;

struct Player
{
    int id;
    std::vector<Vec3> tals;
    Color cursorColor;
    Color dotColor;
};

struct GameState
{
    Vec3 currentPlayerPos;
    Player *currentPlayer;
    Tal currentTal;
    Player player1;
    Player player2;
};

namespace game
{
    GameState *create_state(uint8_t idJoueur1, uint8_t idJoueur2);
    void destroy_state(GameState *);

    void start(GameState* gameState);

    void movePlayer(GameState* gameState, Vec3 move);
    void play_turn(GameState&, RemoteSystem*, Cube*);

    void draw(GameState&, Cube*);
} // namespace game
