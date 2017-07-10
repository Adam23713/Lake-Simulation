#include "waterobject.h"

//Water Object Definition--------------------------------

WaterObject::WaterObject(Point2D position,  WaterObjectType type) : _type(type), _position(position)
{
}

WaterObject::WaterObject(int x, int y, WaterObjectType type) : _type(type), _position(Point2D(x,y))
{
}

 WaterObjectType WaterObject::GetType() const
{
    return _type;
}

void WaterObject::SetPosition(Point2D point)
{
    _position.SetXPosition(point.GetXPosition());
    _position.SetYPosition(point.GetYPosition());
}

void WaterObject::SetPosition(int x, int y)
{
    _position.SetXPosition(x);
    _position.SetYPosition(y);
}

const Point2D& WaterObject::GetPosition() const
{
    return _position;
}
//-------------------------------------------------------
