#include "episodes.h"
#include "cube.h"

#include <cstdio>
#include <unistd.h>
#include <utility>

#include <iostream>

struct ColorF
{
    float r, g, b;
};

static inline float lerp(float a, float b, float t)
{
    return (1 - t)*a + t*b;
}

static inline ColorF blend_color(const ColorF& c1, const ColorF& c2, float t)
{
    return 
    {
        lerp(c1.r, c2.r, t),
        lerp(c1.g, c2.g, t),
        lerp(c1.b, c2.b, t),
    };
}

static inline Color color_float_to_uint(const ColorF& color)
{
    return {(uint8_t)(color.r * 255), (uint8_t)(color.g * 255), (uint8_t)(color.b * 255)};
}

void do_s01e01(Cube* cube)
{
} // anonymous namespace

void do_s01e02(Cube *cube)
{
}

void do_s01e03(Cube *cube)
{
    while (true)
    {
        uint8_t x, y, z;

        std::cout << "X";
        std::cin >> x;

        std::cout << "Y";
        std::cin >> y;

        std::cout << "Z";
        std::cin >> z;

        std::cout << "X : " << x << " y : " << y << " z : " << z << std::endl;

        cube::lightTal(cube, {x, y, z}, {255, 255, 255});
        cube::commit(cube);
    }
}

void do_s01e04(Cube* cube)
{
    ColorF start = {1, 0, 0};
    ColorF end = {1, 1, 0};

    float t = 0;

    for (;;)
    {
        ColorF c = blend_color(start, end, t);

        for (uint8_t x = 1; x < 3; ++x)
        {
            for (uint8_t y = 1; y < 3; ++y)
            {
                for (uint8_t z = 1; z < 3; ++z)
                {
                    cube::lightTal(cube, {x, y, z}, color_float_to_uint(c));
                }
            }
        }

        t += 0.01f;

        if (t > 1.0f)
        {
            std::swap(start, end);
            t = 0.0f;
        }

        usleep(50000);

        cube::commit(cube);
    }
}

namespace episodes
{
    void (*list_of_episodes[])(Cube *) = 
    {
        do_s01e01,
        do_s01e02,
        do_s01e03,
        do_s01e04,
    };

    void start_episode(Cube *cube, Episode episode)
    {
        printf("Starting episode %d\n", episode);
        list_of_episodes[episode](cube);
    }
} // namespace episodes
