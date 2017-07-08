#include "waterobject.h"


//Point2D Definition-------------------------------------
Point2D::Point2D(int x, int y)
{
    if( !SetXPosition(x) && !SetYPosition(y) )
        _x = _y = int(ERROR_CODE::WrongCoordinate);
}

bool Point2D::SetXPosition(int x)
{
    if(x <= 0)
    {
        _x = x;
        return true;
    }
    else
        return false;
}

bool Point2D::SetYPosition(int y)
{
    if(y <= 0)
    {
        _y = y;
        return true;
    }
    else
        return false;
}

int Point2D::GetXPosition() const
{
    return _x;
}

int Point2D::GetYPosition() const
{
    return _y;
}
//-------------------------------------------------------

//Water Object Definition--------------------------------

WaterObject::WaterObject(Point2D position) : _position(position)
{
}

WaterObject::WaterObject(int x, int y) : _position(Point2D(x,y))
{
}

bool WaterObject::SetPosition(Point2D point)
{
    return ( _position.SetXPosition(point.GetXPosition()) && _position.SetYPosition(point.GetYPosition()));
}

bool WaterObject::SetPosition(int x, int y)
{
    return ( _position.SetXPosition(x) && _position.SetYPosition(y));
}

const Point2D& WaterObject::GetPosition() const
{
    return _position;
}
//-------------------------------------------------------
