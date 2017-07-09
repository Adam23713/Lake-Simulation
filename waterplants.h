#ifndef WATERPLANTS_H
#define WATERPLANTS_H

#include "waterobject.h"

enum class LIVE {Withered, Healthy};

class WaterPlant : public WaterObject
{
private:
    LIVE _live;

public:
    //Constructors
    WaterPlant() = delete;
    WaterPlant(Point2D position);
    WaterPlant(int x, int y);

    //Functions
    const LIVE& IsItLive();
    void SetToLiveOrDeath(LIVE value);

};

//Hínár
class Seaweed final : public WaterPlant
{
public:
    Seaweed() = delete;
    Seaweed(Point2D position);
    Seaweed(int x, int y);
};

#endif // WATERPLANTS_H
