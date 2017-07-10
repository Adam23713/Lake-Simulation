#include "waterobject.h"


//Point2D Definition-------------------------------------
Point2D::Point2D(int x, int y)
{
    _x = x;
    _y = y;
}

Point2D::Point2D(const Point2D& P1)
{
    *this = P1;
}

Point2D& Point2D::operator=(const Point2D &P1)
{
    this->_x= P1._x;
    this->_y = P1._y;
    return *this;
}

bool Point2D::operator==(const Point2D& P1) const
{
    if( (this->_x == P1._x) && (this->_y == P1._y) )
        return true;
    else
        return false;
}

bool Point2D::operator<(const Point2D &P1) const
{
    if ( _x == P1._x )
    {
        return _y < P1._y;
    }
    return _x < P1._x;
}

void Point2D::SetXPosition(int x)
{
    _x = x;
}

void Point2D::SetYPosition(int y)
{
    _y = y;
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
