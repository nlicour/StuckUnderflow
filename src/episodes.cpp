#include "episodes.h"
#include "cube.h"

#include <cstdio>
#include <unistd.h>

#include <iostream>

void do_s01e01(Cube *cube)
{
    Color start = {255, 0, 0};
    Color end = {255, 255, 0};

    float t = 0;

    t += 0.0001;
    int32_t level = 0;
    int32_t increment = 10;

    for (;;)
    {
        for (uint8_t x = 1; x < 3; ++x)
        {
            for (uint8_t y = 1; y < 3; ++y)
            {
                for (uint8_t z = 1; z < 3; ++z)
                {
                    cube::ligthTal(cube, {x, y, z}, {level, 255 - level, 0});
                }
            }
        }

        level += increment;
        if (level < 0)
            level = 0;
        if (level > 255)
            level = 255;

        if (level >= 255)
        {
            increment = -increment;
        }

        printf("%d %d\n", level, increment);

        usleep(100000);

        cube::commit(cube);
    }

    /*
    cube::ligthTal(cube, {1, 2, 1}, {255, 255, 255});
    cube::ligthTal(cube, {2, 1, 1}, {255, 255, 255});
    cube::ligthTal(cube, {1, 2, 2}, {255, 255, 255});
    cube::ligthTal(cube, {2, 1, 2}, {255, 255, 255});

    cube::ligthTal(cube, {1, 2, 1}, {255, 255, 255});
    cube::ligthTal(cube, {2, 1, 1}, {255, 255, 255});
    cube::ligthTal(cube, {1, 2, 2}, {255, 255, 255});
    cube::ligthTal(cube, {2, 1, 2}, {255, 255, 255});
    */
} // anonymous namespace

void do_s01e02(Cube *cube)
{
}

void do_s01e03(Cube *cube)
{

    while (true)
    {

        int x, y, z;

        std::cout << "X";
        std::cin >> x;

        std::cout << "Y";
        std::cin >> y;

        std::cout << "Z";
        std::cin >> z;

        std::cout << "X : " << x << " y : " << y << " z : " << z << std::endl;

        cube::ligthTal(cube, {x, y, z}, {255, 255, 255});
        cube::commit(cube);
    }
}

namespace episodes
{
    void (*list_of_episodes[])(Cube *) = {
        do_s01e01,
        do_s01e02,
        do_s01e03,
    };

    void start_episode(Cube *cube, Episode episode)
    {
        printf("Starting episode %d\n", episode);
        list_of_episodes[episode](cube);
    }
} // namespace episodes
