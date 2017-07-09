#include "waterplants.h"

WaterPlant::WaterPlant(Point2D position) : WaterObject(position), _live(LIVE::Healthy)
{
}


WaterPlant::WaterPlant(int x, int y) : WaterObject(Point2D(x,y)), _live(LIVE::Healthy)
{
}

const LIVE& WaterPlant::IsItLive()
{
    return _live;
}

void WaterPlant::SetToLiveOrDeath(LIVE value)
{
    _live = value;
}


//Seaweed definition******************************************************************
Seaweed::Seaweed(Point2D position) : WaterPlant(position)
{

}

Seaweed::Seaweed(int x, int y) : WaterPlant(x,y)
{

}
//************************************************************************************
