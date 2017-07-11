#ifndef WATEROBJECT_H
#define WATEROBJECT_H

#include <string>
#include <vector>
#include "point2d.h"

enum class WaterObjectType { FISH, PLANT };

class WaterObject
{

private:
    bool _live = true;
    WaterObjectType _type;
    Point2D _position;

public:
    WaterObject() = delete;
    WaterObject(Point2D position, WaterObjectType type);
    WaterObject(int x, int y,  WaterObjectType type);
    WaterObject(const WaterObject&) = default;

    void SetLive(bool value);
    bool GetLive() const;
    WaterObjectType GetType() const;
    void SetPosition(int x, int y);
    void SetPosition(Point2D point);
    const Point2D& GetPosition() const;
};

#endif // WATEROBJECT_H
