#include "deadzone.h"

DeadZone::DeadZone(Point2D position, WaterObjectType obj) : WaterObject(position,WaterObjectType::DEADZONE), _onIt(obj)
{

}

DeadZone::DeadZone(int x, int y, WaterObjectType obj) : WaterObject(x,y,WaterObjectType::DEADZONE), _onIt(obj)
{

}

bool DeadZone::drawndIt()
{
    return _drawnIt;
}

void DeadZone::setDrawnIt(bool value)
{
    _drawnIt = value;
}

WaterObjectType DeadZone::WhatIsOnIt() const
{
    return _onIt;
}
