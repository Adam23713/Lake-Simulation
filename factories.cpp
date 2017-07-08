#include "factories.h"

RandomWaterObjectFactory::RandomWaterObjectFactory(std::vector<std::wstring> &speciesList, unsigned int xSize, unsigned int ySize,
                                                   unsigned int fishesNumber, unsigned int plantsNumber)
    : _xSize(xSize), _ySize(ySize), _fishNumber(fishesNumber), _plantsNumber(plantsNumber), _speciesList(speciesList)
{
    _engine.seed(time(nullptr));
}

Fish* RandomWaterObjectFactory::makeFishObject(std::wstring& fishName)
{
    std::uniform_int_distribution<int> x(0,_xSize);
    std::uniform_int_distribution<int> y(0,_ySize);
    std::uniform_int_distribution<int> size(1,3);

    if(fishName == L"Kárász")
        return new CrucianCarp(x(_engine),y(_engine),size(_engine));
    else if(fishName == L"Csuka")
       return new NorthernPike(x(_engine),y(_engine),size(_engine));
    else if(fishName == L"Fejes domolykó")
        return new EuropeanChub(x(_engine),y(_engine),size(_engine));
    else
        return new SpecialFish(Point2D(x(_engine),y(_engine)),size(_engine),SPECIES::Omnivorous,fishName);
}

std::vector<WaterObject *> RandomWaterObjectFactory::makeWaterObjectVector()
{

    std::vector<WaterObject *> lakeElements;

    for(auto i : AvailableFishes)
    {
        lakeElements.push_back(makeFishObject(i));
    }

    for(unsigned int i = 0; i < _fishNumber - AvailableFishes.size(); i++)
    {
        //function
    }

    for(unsigned int i = 0; i < _plantsNumber; i++)
    {

    }

    return lakeElements;
}
