#include "factories.h"

RandomWaterObjectFactory::RandomWaterObjectFactory(std::vector<std::wstring> &speciesList, unsigned int xSize, unsigned int ySize, unsigned int fishesNumber, unsigned int plantsNumber)
    : _xSize(xSize), _ySize(ySize), _fishNumber(fishesNumber), _plantsNumber(plantsNumber)
{
    //Set the random number genereator's seed
    _engine.seed(time(nullptr));


    for(unsigned int i = 0; i < _xSize; i++)
    {
        for(unsigned int j = 0; j < _ySize; i++)
        {
            _locationsMap[Point2D(i,j)] = false;
        }
    }

    for(auto i : speciesList)
    {
        _availableFishes.push_back(i);
    }
}

Fish* RandomWaterObjectFactory::makeFishObject(std::wstring& fishName)
{
    std::uniform_int_distribution<int> x(0,_xSize);
    std::uniform_int_distribution<int> y(0,_ySize);
    std::uniform_int_distribution<int> size(1,3);

    //Is the generated point in it?
    Point2D randomPoint = Point2D(x(_engine),y(_engine));
    while( _locationsMap[randomPoint] != true )
        randomPoint = Point2D(x(_engine),y(_engine));

    //Which fish it generates
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

    std::vector<WaterObject*> lakeElements;

    //Generate all fish species
    for(auto i : _availableFishes)
    {
        lakeElements.push_back(makeFishObject(i));
    }

    //Generate random fishies
    std::uniform_int_distribution<int> randNumber(0,_availableFishes.size());
    for(unsigned int i = 0; i < _fishNumber - _availableFishes.size(); i++)
    {
        lakeElements.push_back( makeFishObject(_availableFishes.at(randNumber(_engine))));
    }

    //Generate water plants
    std::uniform_int_distribution<int> randPositionX(0,_xSize);
    std::uniform_int_distribution<int> randPositionY(0,_ySize);
    for(unsigned int i = 0; i < _plantsNumber; i++)
    {
        lakeElements.push_back(new Seaweed(randPositionX(_engine),randPositionY(_engine)));
    }

    return lakeElements;
}
