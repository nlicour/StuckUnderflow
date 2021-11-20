#include "cube.h"
#include <vector>

using namespace std;

struct Cube
{
    std::vector<Tal> tals; 
};

struct Tal
{
    Vec3 pos;
    Color leds[3];
};

namespace cube
{
    void ligthTal(Cube* cube, Vec3 tal, Color color)
    {
        for (unsigned int i = 0; i < 3; i++)
        {
            cube::ligthLed(cube, tal, i, color);
        }
    }

    void ligthLed(Cube* cube, Vec3 talPos, unsigned int led, Color color)
    {
        for (Tal tmpTal : cube->tals)
        {
            if (tmpTal.pos == talPos)
            {
                tmpTal.leds[led].r = color.r;
                tmpTal.leds[led].g = color.g;
                tmpTal.leds[led].b = color.b;
            }
        }
    }
}