#include "point2d.h"

//Point2D Definition-------------------------------------
float Point2D::GetDistance(const Point2D &P1, const Point2D &P2)
{
    return std::sqrt(std::pow((P2._x - P1._x),2) + std::pow((P2._y - P1._y),2));
}

Point2D::Point2D(int x, int y)
{
    _x = x;
    _y = y;
}

Point2D::Point2D(const Point2D& P1)
{
    *this = P1;
}

Point2D &Point2D::operator+(const Point2D &P1)
{
    this->_x = this->_x + P1._x;
    this->_y = this->_y + P1._y;
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
