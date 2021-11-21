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

    // renvoie 0 si personne n'a gagné, 
    // 1 si le joueur 1 a gagné,
    // 2 si le joueur 2 a gagné, 
    // 3 si égalité 
    int checkGrid(GameState *gs)
    {
        int resPlayer1; 
        int resPlayer2;
        Color white = {255, 255, 255};
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                // Test en ligne 
                if (gs->colorGrid[16*i + 4*j] == gs->colorGrid[16*i + 4*j +1] &&
                gs->colorGrid[16*i + 4*j] == gs->colorGrid[16*i + 4*j +2] &&
                gs->colorGrid[16*i + 4*j] == gs->colorGrid[16*i + 4*j +3] &&
                !(gs->colorGrid[16*i + 4*j] == white))
                {
                    gs->colorGrid[16*i + 4*j] == gs->player1.dotColor ? resPlayer1++ : resPlayer2 += 2;
                }

                // Test en colonne 
                if (gs->colorGrid[16*i + j] == gs->colorGrid[16*i + 4 + j] &&
                    gs->colorGrid[16*i + j] == gs->colorGrid[16*i + 8 + j] &&
                    gs->colorGrid[16*i + j] == gs->colorGrid[16*i + 12 + j] &&
                    !(gs->colorGrid[16*i + j] == white))
                {
                    gs->colorGrid[16*i + j] == gs->player1.dotColor ? resPlayer1++ : resPlayer2 += 2;
                }
            }
            // Diagonale gauche 
            if (gs->colorGrid[16*i] == gs->colorGrid[16*i + 4 + 1] &&
                gs->colorGrid[16*i] == gs->colorGrid[16*i + 8 + 2] &&
                gs->colorGrid[16*i] == gs->colorGrid[16*i + 12 + 3] &&
                !(gs->colorGrid[16*i] == white))
            {
                gs->colorGrid[16*i] == gs->player1.dotColor ? resPlayer1++ : resPlayer2 += 2;
            }

            // Diagonale droite   
            if (gs->colorGrid[16*i + 4 - 1] == gs->colorGrid[16*i + 8 - 2] &&
                gs->colorGrid[16*i + 4 - 1] == gs->colorGrid[16*i + 12 - 3] &&
                gs->colorGrid[16*i + 4 - 1] == gs->colorGrid[16*i + 16 - 4] &&
                !(gs->colorGrid[16*i + 4 - 1] == white))
            {
                gs->colorGrid[16*i + 4 - 1] == gs->player1.dotColor ? resPlayer1++ : resPlayer2 += 2;
            }
        }
        return resPlayer1 + resPlayer2;
    }
} // namespace game
