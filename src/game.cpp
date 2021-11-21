#include <iostream>
#include "game.h"
#include "cube.h"
#include "remote.h"

namespace game
{
    GameState *create_state(uint8_t idJoueur1, uint8_t idJoueur2)
    {
        GameState *game = new GameState();
        game->player1.id = idJoueur1;
        game->player2.id = idJoueur2;

        return game;
    }

    void destroy_state(GameState *rs)
    {
        delete rs;
    }

    void start(GameState *gameState)
    {
    }

    void movePlayer(GameState *gameState, Vec3 move, Cube *cube)
    {
        gameState->currentPlayerPos.x = (gameState->currentPlayerPos.x + (move.x % 2)) % 4;
        gameState->currentPlayerPos.y = (gameState->currentPlayerPos.y + (move.y % 2)) % 4;
        gameState->currentPlayerPos.z = (gameState->currentPlayerPos.z + (move.z % 2)) % 4;

        draw(gameState, cube);
    }

    void pasteDot(GameState *gameState, Cube *cube)
    {
        Player *player = gameState->currentPlayer;
        player->tals.push_back(gameState->currentPlayerPos);

        draw(gameState, cube);
    }

    void play_turn(GameState &gs, RemoteSystem *rs, Cube *cube)
    {
        Vec3 move = {0, 0, 0};
        for (uint8_t i = 0; i < 3; i++)
        {
            uint16_t button = remote::wait_for_state_change(rs, gs.currentPlayer->id);

            if (button)
            {
                std::cout << "Recieved from " << gs.currentPlayer->id << " - " << button << std::endl;
                move.x = button & 0x1 ? 1 : 0;
                move.y = button & 0x2 ? 1 : 0;
                move.z = button & 0x4 ? 1 : 0;

                movePlayer(&gs, move, cube);
                // valide le jeton
                if (button & 0x8)
                {
                    pasteDot(&gs, cube);
                    break;
                }
            }
        }
    }

    void drawPlayer(Player &player, Cube *cube)
    {
        for (auto tal : player.tals)
        {
            cube::lightTal(cube, tal, player.dotColor);
        }
    }

    void draw(GameState *gs, Cube *cube)
    {
        drawPlayer(gs->player1, cube);
        drawPlayer(gs->player2, cube);
        cube::lightTal(cube, gs->currentPlayerPos, gs->currentPlayer->cursorColor);
        cube::commit(cube);
    }
} // namespace game
