#ifndef WATEROBJECT_H
#define WATEROBJECT_H

#include <string>
#include <vector>
#include "point2d.h"

enum class WaterObjectType { FISH, PLANT, DEADZONE };

class WaterObject
{

private:
    //bool _live = true;
    WaterObjectType _type;
    Point2D _position;

public:
    WaterObject() = delete;
    virtual ~WaterObject() = default;
    WaterObject(Point2D position, WaterObjectType type);
    WaterObject(int x, int y,  WaterObjectType type);
    WaterObject(const WaterObject&) = default;

    WaterObjectType GetType() const;
    void SetPosition(int x, int y);
    void SetPosition(Point2D point);
    const Point2D& GetPosition() const;
};

#endif // WATEROBJECT_H
