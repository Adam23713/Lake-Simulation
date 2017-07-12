#ifndef FACTORIES_H
#define FACTORIES_H

#include <random>
#include <vector>
#include <map>
#include <string>
#include "waterobject.h"
#include "fishes.h"
#include "waterplants.h"

//Abstract Factory****************************************************
class Factory
{
public:
    virtual ~Factory() = default;
    virtual std::vector<WaterObject*> makeWaterObjectVector() = 0;
};
//********************************************************************

class RandomWaterObjectFactory : public Factory
{
private:
    std::map<Point2D,bool> _locationsMap;
    std::vector<std::wstring> _availableFishes = {L"Kárász",L"Csuka",L"Fejes domolykó"};
    const std::vector<std::wstring> AvailablePlants = {L"Hínár"};

    unsigned int _xSize;
    unsigned int _ySize;
    unsigned int _fishNumber;
    unsigned int _plantsNumber;
    std::mt19937 _engine;

    //Private functions
    Point2D generateRandomPoint();
    Fish *makeFishObject(std::wstring &fishName);

public:
    ~RandomWaterObjectFactory();
    RandomWaterObjectFactory() = delete;
    RandomWaterObjectFactory(std::vector<std::wstring>& speciesList, unsigned int xSize, unsigned int ySize,
                             unsigned int fishesNumber = 100, unsigned int plantsNumber = 30);

    std::vector<WaterObject*> makeWaterObjectVector() override;
};

#endif // FACTORIES_H
