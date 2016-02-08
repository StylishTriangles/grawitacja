#include "celestialbody.h"

celestialBody::celestialBody() :
    bStatic(false),
    mass(1),
    posx(0),
    posy(0),
    vx(0),
    vy(0),
    width(0),
    height(0),
    brush(Qt::magenta),
    pen(Qt::darkYellow)
{

}

void celestialBody::makePlanet(long double x, long double y)
{
    mass = 8500;
    posx = x;
    posy = y;
    vx = 0;
    vy = 0;
    width = 8;
    height = 8;
}
