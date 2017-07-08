#ifndef FACTORIES_H
#define FACTORIES_H

#include <random>
#include <vector>
#include "waterobject.h"
#include "fishes.h"
#include "waterplants.h"

//Abstract Factory***********************************
class Factori
{
public:
    virtual std::vector<WaterObject*> makeWaterObjectVector() = 0;
};
//***************************************************

class RandomWaterObjectFactory : public Factori
{
private:
    const std::vector<std::wstring> AvailableFishes = {L"Kárász",L"Csuka",L"Fejes domolykó"};
    const std::vector<std::wstring> AvailablePlants = {L"Hínár"};

    unsigned int _xSize;
    unsigned int _ySize;
    unsigned int _fishNumber;
    unsigned int _plantsNumber;
    std::vector<std::wstring>& _speciesList;
    std::mt19937 _engine;

    //Private functions
    Fish *makeFishObject(std::wstring &fishName);

public:
    RandomWaterObjectFactory() = delete;
    RandomWaterObjectFactory(std::vector<std::wstring>& speciesList, unsigned int xSize, unsigned int ySize,
                             unsigned int fishesNumber = 100, unsigned int plantsNumber = 30);

    std::vector<WaterObject*> makeWaterObjectVector() override;
};

#endif // FACTORIES_H
