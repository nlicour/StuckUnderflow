#pragma once
//
#include <cstdint>
//
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
    void ligthTal(Cube* cube, Vec3 tal, Color color);

    void ligthLed(Cube* cube, Vec3 tal, unsigned int led, Color color);
}
