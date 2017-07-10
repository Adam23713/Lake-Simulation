#ifndef WATEROBJECT_H
#define WATEROBJECT_H

#include <string>

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
    void SetXPosition(int x);
    void SetYPosition(int y);
    int GetXPosition() const;
    int GetYPosition() const;
};

enum class WaterObjectType { FISH, PLANT };

class WaterObject
{

private:
    WaterObjectType _type;
    Point2D _position;

public:
    WaterObject() = delete;
    WaterObject(Point2D position, WaterObjectType type);
    WaterObject(int x, int y,  WaterObjectType type);
    WaterObject(const WaterObject&) = default;

    WaterObjectType GetType() const;
    void SetPosition(int x, int y);
    void SetPosition(Point2D point);
    const Point2D& GetPosition() const;
};

#endif // WATEROBJECT_H
