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
};

namespace cube
{
    void ligthTal(Cube* cube, Vec3 tal, Color color);

    
}
