#ifndef WATEROBJECT_H
#define WATEROBJECT_H

#include <string>

enum class ERROR_CODE : int {WrongCoordinate = -1};

class Point2D
{
private:
    int _x;
    int _y;

public:
    //Constructors
    Point2D() = delete;
    Point2D(int x, int y);
    Point2D(const Point2D &P1);

    //Operators
    Point2D& operator=(const Point2D& P1);
    bool operator==(const Point2D& P1) const;
    bool operator<( const Point2D& P1) const;

    //Functions
    bool SetXPosition(int x);
    bool SetYPosition(int y);
    int GetXPosition() const;
    int GetYPosition() const;
};

class WaterObject
{

private:
    Point2D _position;

public:
    WaterObject() = delete;
    WaterObject(Point2D position);
    WaterObject(int x, int y);
    WaterObject(const WaterObject&) = default;

    bool SetPosition(int x, int y);
    bool SetPosition(Point2D point);
    const Point2D& GetPosition() const;
};

#endif // WATEROBJECT_H
