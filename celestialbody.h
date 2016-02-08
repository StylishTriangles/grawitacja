#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include <QObject>
#include <QColor>
#include <cmath>
#include <cinttypes>
#include <algorithm>

class celestialBody
{
public:
    enum TYPES {DEFAULT, PLANET=0, SUN, BLACK_HOLE};

    celestialBody();
    inline void toggleImmovable()                                       {bStatic=!bStatic;}
    inline void setMass(long double m)                                  {mass = m;}
    inline void setPos(long double x, long double y)                    {posx=x;posy=y;}
    inline void setVelocity(long double _vx,long double _vy)            {vx= _vx;vy=_vy;}
    inline void setVx(long double _vx)                                  {vx=_vx;}
    inline void setVy(long double _vy)                                  {vy=_vy;}
    inline void setSize(short _width, short _height)                    {width=_width;height=_height;}
    //inline void setVelocity(long double v, long double angle) {vx=v*cos(angle);vy=v*sin(angle);}
    inline void move(int timeMillis)                                    {if(bStatic) return; posx+=vx*timeMillis/1000.0;posy+=vy*timeMillis/1000.0;}
    inline void moveX(long double distance)                             {posx+=distance;}
    inline void moveY(long double distance)                             {posy+=distance;}
    inline void changeVelocity(long double deltavx, long double deltavy){vx+=deltavx;vy+=deltavy;}
    inline void setBrushColor(QColor c)                                 {brush = c;}
    inline void setPenColor(QColor c)                                   {pen = c;}

    inline bool immovable() const                                   {return bStatic;}
    inline long double getMass() const                              {return mass;}
    inline std::pair<long double,long double> getPos() const        {return {posx,posy};}
    inline long double getX() const                                 {return posx;}
    inline long double getY() const                                 {return posy;}
    inline void getPos(long double& x, long double& y) const        {x = posx; y = posy;}
    inline short getWidth() const                                   {return width;}
    inline short getHeight() const                                  {return height;}
    inline short getRadius() const                                  {return std::min(height,width);}
    inline std::pair<long double,long double> getVelocity() const   {return {vx,vy};}
    inline long double getVx() const                                {return vx;}
    inline long double getVy() const                                {return vy;}
    inline std::pair<short,short> getSize() const                   {return {width,height};}

    void makePlanet(long double x, long double y);
    inline long double distance_2(celestialBody const& cb) const    {return (posx-cb.getX())*(posx-cb.getX())+(posy-cb.getY())*(posy-cb.getY());}
    inline long double distance(celestialBody const& cb) const      {return sqrtl(distance_2(cb));}
    inline QColor const& getBrushColor() const                      {return brush;}
    inline QColor const& getPenColor() const                        {return pen;}
protected:
    bool bStatic;
    long double mass;
    long double posx;
    long double posy;
    long double vx;
    long double vy;
    short width;
    short height;
    QColor brush;
    QColor pen;
};

#endif // CELESTIALBODY_H
