#include "remote.h"
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

void do_s01e01(Cube* cube, RemoteSystem* remote)
{
} // anonymous namespace

void do_s01e02(Cube *cube, RemoteSystem* remote)
{
}

void do_s01e03(Cube *cube, RemoteSystem* remote)
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

void do_s01e04(Cube* cube, RemoteSystem* remote)
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

void do_multi_remotes(Cube* cube, RemoteSystem* remote)
{
    if (!remote::connect(remote, 0))
    {
        fprintf(stderr, "Couldn't connect to remote %d\n", 0);
    }

    if (!remote::connect(remote, 1))
    {
        fprintf(stderr, "Couldn't connect to remote %d\n", 1);
    }

    auto fill_cube = [&](Color fill_color){
        for (uint8_t x = 0; x < 4; ++x)
        {
            for (uint8_t y = 0; y < 4; ++y)
            {
                for (uint8_t z = 0; z < 4; ++z)
                {
                    cube::lightTal(cube, {x, y, z}, fill_color);
                }
            }
        }

        cube::commit(cube);
    };

    uint16_t buttons_1;
    uint16_t buttons_2;
    for(;;)
    {
        bool remote_1_has_event = false;
        bool remote_2_has_event = false;

        while (remote::poll_remote(remote, 0, &buttons_1))
        {
            remote_1_has_event = true;
        }

        while (remote::poll_remote(remote, 1, &buttons_2))
        {
            remote_2_has_event = true;
        }

        if (remote_1_has_event)
        {
            fill_cube({255, 0, 0});
            printf("Remote 1 received: %d\n", buttons_1);
        }
        if (remote_2_has_event)
        {
            fill_cube({0, 255, 0});
            printf("Remote 2 received: %d\n", buttons_2);
        }
    }
}

namespace episodes
{
    void (*list_of_episodes[])(Cube *, RemoteSystem* remote) = 
    {
        do_multi_remotes,
        do_s01e01,
        do_s01e02,
        do_s01e03,
        do_s01e04,
    };

    void start_episode(Cube *cube, RemoteSystem* remote, Episode episode)
    {
        printf("Starting episode %d\n", episode);
        list_of_episodes[episode](cube, remote);
    }
} // namespace episodes
