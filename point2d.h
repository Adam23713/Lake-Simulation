#ifndef POINT2D_H
#define POINT2D_H

#include <cmath>

class Point2D
{
private:
    int _x;
    int _y;

public:
    static float GetDistance(const Point2D& P1, const Point2D& P2);

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

#endif // POINT2D_H
