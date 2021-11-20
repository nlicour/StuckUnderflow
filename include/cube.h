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
};

namespace cube
{
    Cube* create();
    void destroy(Cube*);

    // Initialize a cube by setting up the socket connection, creating the universes, etc.
    bool init(Cube*);

    void ligthTal(Cube* cube, Vec3 tal, Color color);
} // namespace cube
