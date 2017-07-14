#ifndef DEADZONE_H
#define DEADZONE_H

#include "waterobject.h"

class DeadZone : public WaterObject
{

private:
    bool _drawnIt = false;
    WaterObjectType _onIt = WaterObjectType::DEADZONE;

public:
    ~DeadZone() = default;
    DeadZone() = delete;
    DeadZone(Point2D position, WaterObjectType obj);
    DeadZone(int x, int y, WaterObjectType obj);

    //Public function
    bool drawndIt();
    void setDrawnIt(bool value);
    WaterObjectType WhatIsOnIt() const;
};

#endif // DEADZONE_H
