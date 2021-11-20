#pragma once

#include <cstdint>

class Cube;

struct Color
{
    uint8_t r, g, b;
};

struct Vec3
{
    uint8_t x, y, z;

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

    void commit(Cube *cube);
} // namespace cube
