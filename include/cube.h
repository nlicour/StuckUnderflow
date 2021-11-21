#pragma once

#include <cstdint>

class Cube;

struct Color
{
    uint8_t r, g, b;

    bool operator==(const Color& other)
    {
        return  r == other.r &&
                g == other.g &&
                b == other.b;
    }
};

struct Tal
{
    int index;
    Color leds[3];
};

struct Vec3
{
    uint32_t x, y, z;

    bool operator==(const Vec3& other)
    {
        return  x == other.x &&
                y == other.y &&
                z == other.z;
    }
};

namespace cube
{
    Cube* create();
    void destroy(Cube*);

    // Initialize a cube by setting up the socket connection, creating the universes, etc.
    bool init(Cube*);


    void lightLed(Cube* cube, Vec3 tal, uint8_t led, Color color);

    void lightTal(Cube* cube, Vec3 tal, Color color);

    void lightLayer(Cube* cube, uint8_t layer, Color color);

    void commit(Cube *cube);
} // namespace cube
