#include "factories.h"

RandomWaterObjectFactory::RandomWaterObjectFactory(std::vector<std::wstring> &speciesList, unsigned int xSize, unsigned int ySize, unsigned int fishesNumber, unsigned int plantsNumber)
    : _xSize(xSize), _ySize(ySize), _fishNumber(fishesNumber), _plantsNumber(plantsNumber)
{
    //Set the random number genereator's seed
    _engine.seed(time(nullptr));

    for(unsigned int i = 0; i < _xSize; i++)
        for(unsigned int j = 0; j < _ySize; j++)
            _locationsMap[Point2D(i,j)] = false;

    for(auto i : speciesList)
        _availableFishes.push_back(i);
}

Point2D RandomWaterObjectFactory::generateRandomPoint()
{
    Point2D randomPoint(0,0);
    std::uniform_int_distribution<int> x(0,_xSize-1);
    std::uniform_int_distribution<int> y(0,_ySize-1);
    while( true )
    {
        randomPoint = Point2D(x(_engine),y(_engine));
        if( _locationsMap[randomPoint] == false )
        {
            _locationsMap[randomPoint] = true;
            break;
        }
    }
    return randomPoint;
}

Fish* RandomWaterObjectFactory::makeFishObject(std::wstring& fishName)
{

    std::uniform_int_distribution<int> size(1,3);
    //Is the generated point in it?
    Point2D randomPoint = generateRandomPoint();

    //Which fish it generates
    if(fishName == L"Kárász")
        return new CrucianCarp(randomPoint.GetXPosition(),randomPoint.GetYPosition(),size(_engine));
    else if(fishName == L"Csuka")
       return new NorthernPike(randomPoint.GetXPosition(),randomPoint.GetYPosition(),size(_engine));
    else if(fishName == L"Fejes domolykó")
        return new EuropeanChub(randomPoint.GetXPosition(),randomPoint.GetYPosition(),size(_engine));
    else
        return new SpecialFish(randomPoint.GetXPosition(),randomPoint.GetYPosition(),size(_engine),SPECIES::Omnivorous,fishName);
}

RandomWaterObjectFactory::~RandomWaterObjectFactory()
{

}

std::vector<WaterObject *> RandomWaterObjectFactory::makeWaterObjectVector()
{
    std::vector<WaterObject*> lakeElements;

    if((_xSize * _ySize) < (_fishNumber + _plantsNumber))
        return lakeElements;

    //Generate all fish species*****************************************************************
    for(auto i : _availableFishes)
    {
        lakeElements.push_back(makeFishObject(i));
    }
    //******************************************************************************************

    //Generate random fishies--------------------------------------------------------------------
    std::uniform_int_distribution<int> randNumber(0,_availableFishes.size()-1);
    for(unsigned int i = 0; i < _fishNumber - _availableFishes.size(); i++)
    {
        lakeElements.push_back( makeFishObject(_availableFishes.at(randNumber(_engine))));
    }
    //-------------------------------------------------------------------------------------------

    //Generate water plants++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    for(unsigned int i = 0; i < _plantsNumber; i++)
    {
        Point2D randomPoint = generateRandomPoint();
        lakeElements.push_back( new Seaweed(randomPoint.GetXPosition(),randomPoint.GetYPosition()) );
    }
    //++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

    return lakeElements;
}
