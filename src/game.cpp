#include <iostream>
#include "game.h"
#include "cube.h"
#include "remote.h"

#include <cstdio>
#include <unistd.h>
#include <utility>

#define MS_TO_US(ms) (ms * 1000)

namespace
{
    struct ColorF
    {
        float r, g, b;
    };

    static inline float lerp(float a, float b, float t)
    {
        return (1 - t) * a + t * b;
    }

    static inline ColorF blend_color(const ColorF &c1, const ColorF &c2, float t)
    {
        return {
            lerp(c1.r, c2.r, t),
            lerp(c1.g, c2.g, t),
            lerp(c1.b, c2.b, t),
        };
    }

    static inline Color color_float_to_uint(const ColorF &color)
    {
        return {(uint8_t)(color.r * 255), (uint8_t)(color.g * 255), (uint8_t)(color.b * 255)};
    }

    void clear_cube(Cube *cube)
    {
        for (uint8_t x = 0; x < 4; ++x)
        {
            for (uint8_t y = 0; y < 4; ++y)
            {
                for (uint8_t z = 0; z < 4; ++z)
                {
                    cube::lightTal(cube, {x, y, z}, {0, 0, 0});
                }
            }
        }

        cube::commit(cube);
    }

    void do_start_animation(GameState *gs, Cube *cube)
    {
        clear_cube(cube);

        ColorF start = {0, 1, 1};
        ColorF end = {0.8, 0.5, 0};

        float t = 0;
        for (uint32_t i = 0; i < 100; ++i)
        {
            for (uint8_t x = 0; x < 4; ++x)
            {
                for (uint8_t y = 0; y < 4; ++y)
                {
                    for (uint8_t z = 0; z < 4; ++z)
                    {
                        cube::lightTal(cube, {x, y, z}, {255, 255, 255});
                    }
                }
            }

            ColorF c = blend_color(start, end, t);

            for (uint8_t x = 0; x < 4; ++x)
            {
                if (x == 0 || x == 3)
                    continue;

                for (uint8_t y = 0; y < 4; ++y)
                {
                    if (y == 0 || y == 3)
                        continue;

                    for (uint8_t z = 0; z < 4; ++z)
                    {
                        if (z == 0 || z == 3)
                            continue;

                        cube::lightTal(cube, {x, y, z}, color_float_to_uint(c));
                    }
                }
            }

            t += 0.05;
            if (t >= 1.0f)
            {
                std::swap(end, start);
                t = 0.0f;
            }

            usleep(50000);

            cube::commit(cube);
        }

        printf("Done\n");
    }

    void do_end_animation(Cube *cube, Color winner)
    {
        clear_cube(cube);
        Color black = {0, 0, 0};

        for (unsigned int i = 0; i < 20; i++)
        {
            for (uint8_t x = 0; x < 4; ++x)
            {
                for (uint8_t y = 0; y < 4; ++y)
                {
                    for (uint8_t z = 0; z < 4; ++z)
                    {
                        if (x == 0 || x == 3 || y == 0 || y == 3 || z == 0 || z == 3)
                        {
                            cube::lightTal(cube, {x, y, z}, i % 2 == 0 ? black : winner);
                        }
                        else
                        {
                            cube::lightTal(cube, {x, y, z}, i % 2 == 0 ? winner : black);
                        }
                    }
                }
            }
            cube::commit(cube);
            usleep(250000);
        }

        for (uint8_t x = 0; x < 4; ++x)
        {
            for (uint8_t y = 0; y < 4; ++y)
            {
                for (uint8_t z = 0; z < 4; ++z)
                {
                    cube::lightTal(cube, {x, y, z}, winner);
                }
            }
        }
        cube::commit(cube);
    }
} // anonymous namespace

void updateGrid(GameState *gameState)
{

    // gameState->cube
}

namespace game
{
    GameState *create_state(uint8_t idJoueur1, uint8_t idJoueur2)
    {
        GameState *game = new GameState();
        game->player1.id = idJoueur1;
        game->player2.id = idJoueur2;

        game->player1.dotColor = {255, 128, 0};
        game->player1.cursorColor = {255, 0, 0};

        game->player2.dotColor = {0, 255, 255};
        game->player2.cursorColor = {0, 0, 255};

        game->colorGrid.resize(64);

        return game;
    }

    void destroy_state(GameState *rs)
    {
        delete rs;
    }

    void reset(GameState *gs)
    {
        gs->run_start_animation = true;
        gs->run_end_animation = false;
    }

    void movePlayer(GameState *gameState, Vec3 move, Cube *cube)
    {
        gameState->currentPlayerPos.x = (gameState->currentPlayerPos.x + move.x) % 4;
        gameState->currentPlayerPos.y = (gameState->currentPlayerPos.y + move.y) % 4;
        gameState->currentPlayerPos.z = (gameState->currentPlayerPos.z + move.z) % 4;

        while (gameState->currentPlayerPos.x < 0) gameState->currentPlayerPos.x += 4;
        while (gameState->currentPlayerPos.y < 0) gameState->currentPlayerPos.y += 4;
        while (gameState->currentPlayerPos.z < 0) gameState->currentPlayerPos.z += 4;

        /*
        gameState->currentPlayerPos.x = gameState->currentPlayerPos.x < 0 ? 0 : gameState->currentPlayerPos.x;
        gameState->currentPlayerPos.y = gameState->currentPlayerPos.y < 0 ? 0 : gameState->currentPlayerPos.y;
        gameState->currentPlayerPos.z = gameState->currentPlayerPos.z < 0 ? 0 : gameState->currentPlayerPos.z;
        */

        printf("Player: %d %d %d\n", gameState->currentPlayerPos.x, gameState->currentPlayerPos.y, gameState->currentPlayerPos.z);

        draw(gameState, cube);
    }

    void pasteDot(GameState *gameState, Cube *cube)
    {
        Player *player = gameState->currentPlayer;
        player->tals.push_back(gameState->currentPlayerPos);
        gameState->colorGrid[cube::vec3_to_tal_index(gameState->currentPlayerPos)];

        draw(gameState, cube);
    }

    void drawPlayer(Player &player, Cube *cube)
    {
        for (auto tal : player.tals)
        {
            cube::lightTal(cube, tal, player.dotColor);
        }
    }

    void lightLayer(Cube* cube, uint8_t z)
    {
        if (z > 3) return;

        for (uint8_t x = 0; x < 4; ++x)
        {
            for (uint8_t y = 0; y < 4; ++y)
            {
                cube::lightTal(cube, {x, y, z}, {100, 100, 100});
            }
        }
    }

    void draw(GameState* gs, Cube* cube)
    {
        if (gs->run_start_animation)
        {
            do_start_animation(gs, cube);
            gs->run_start_animation = false;
            draw(gs, cube);
        }
        else if (gs->run_end_animation)
        {
            do_end_animation(cube, gs->currentPlayer->dotColor);
        }
        else
        {
            lightLayer(cube, gs->currentPlayerPos.z);
            drawPlayer(gs->player1, cube);
            drawPlayer(gs->player2, cube);
            cube::lightTal(cube, gs->currentPlayerPos, gs->currentPlayer->cursorColor);
            cube::commit(cube);
        }
    }

    bool is_game_running(GameState *gs)
    {
        return !gs->run_start_animation && !gs->run_end_animation;
    }

    // renvoie 0 si personne n'a gagné,
    // 1 si le joueur 1 a gagné,
    // 2 si le joueur 2 a gagné,
    // 3 si égalité
    int checkGrid(GameState *gs)
    {
        int resPlayer = 0;

        Color black = {0, 0, 0};
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                // Test en ligne
                if (gs->colorGrid[16 * i + 4 * j] == gs->colorGrid[16 * i + 4 * j + 1] &&
                    gs->colorGrid[16 * i + 4 * j] == gs->colorGrid[16 * i + 4 * j + 2] &&
                    gs->colorGrid[16 * i + 4 * j] == gs->colorGrid[16 * i + 4 * j + 3] &&
                    !(gs->colorGrid[16 * i + 4 * j] == black))
                {
                    resPlayer = gs->colorGrid[16 * i + 4 * j] == gs->player1.dotColor ? 1 : 2;
                }

                // Test en colonne
                if (gs->colorGrid[16 * i + j] == gs->colorGrid[16 * i + 4 + j] &&
                    gs->colorGrid[16 * i + j] == gs->colorGrid[16 * i + 8 + j] &&
                    gs->colorGrid[16 * i + j] == gs->colorGrid[16 * i + 12 + j] &&
                    !(gs->colorGrid[16 * i + j] == black))
                {
                    resPlayer = gs->colorGrid[16 * i + j] == gs->player1.dotColor ? 1 : 2;
                }
            }
            // Diagonale gauche
            if (gs->colorGrid[16 * i] == gs->colorGrid[16 * i + 4 + 1] &&
                gs->colorGrid[16 * i] == gs->colorGrid[16 * i + 8 + 2] &&
                gs->colorGrid[16 * i] == gs->colorGrid[16 * i + 12 + 3] &&
                !(gs->colorGrid[16 * i] == black))
            {
                resPlayer = gs->colorGrid[16 * i] == gs->player1.dotColor ? 1 : 2;
            }

            // Diagonale droite
            if (gs->colorGrid[16 * i + 4 - 1] == gs->colorGrid[16 * i + 8 - 2] &&
                gs->colorGrid[16 * i + 4 - 1] == gs->colorGrid[16 * i + 12 - 3] &&
                gs->colorGrid[16 * i + 4 - 1] == gs->colorGrid[16 * i + 16 - 4] &&
                !(gs->colorGrid[16 * i + 4 - 1] == black))
            {
                resPlayer = gs->colorGrid[16 * i + 4 - 1] == gs->player1.dotColor ? 1 : 2;
            }
        }
        return resPlayer;
    }

    bool play_turn(GameState &gs, RemoteSystem *rs, Cube *cube)
    {
        remote::toggle_led(rs, gs.currentPlayer->id, 7);

        Vec3 move = {0, 0, 0};
        bool hasValidatedMove = false;
        while (!hasValidatedMove)
        {
            uint16_t button = remote::wait_for_state_change(rs, gs.currentPlayer->id);

            if (button)
            {
                move.x = button == 512 ? 1 : (button == 64 ? -1 : 0);
                move.y = button == 32 ? 1 : (button == 128 ? -1 : 0);
                move.z = button == 2 ? 1 : (button == 1 ? -1 : 0);

                // move.x = button & 0x1 ? 1 : (button & 0x8 ? -1 : 0);
                // move.y = button & 0x2 ? 1 : (button & 0x16 ? -1 : 0);
                // move.z = button & 0x4 ? 1 : (button & 0x32 ? -1 : 0);

                movePlayer(&gs, move, cube);
                // valide le jeton
                if (button & 0x4)
                {
                    pasteDot(&gs, cube);
                    if (checkGrid(&gs) > 0)
                    {
                        return true;
                    }
                    hasValidatedMove = true;
                }
                else if ((button & 0x10) || (button & 0x8))
                {
                    gs.run_end_animation = true;
                    break;
                }
            }
        }

        remote::toggle_led(rs, gs.currentPlayer->id, 0);
        return false;
    }
} // namespace game
