#include "fishes.h"


//Fish Definition----------------------------------------------------------------------------------------------------
Fish::Fish(Point2D position, unsigned char size, SPECIES spec) : WaterObject(position, WaterObjectType::FISH), _size(size), _species(spec)
{

}

Fish::Fish(int x, int y, unsigned char size, SPECIES spec) : WaterObject(Point2D(x,y),WaterObjectType::FISH), _size(size), _species(spec)
{

}

SPECIES Fish::GetSpecies() const
{
    return _species;
}

unsigned char Fish::GetSize() const
{
    return _size;
}

void Fish::Move()
{
}

void Fish::Eat(WaterObject&)
{
}
//----------------------------------------------------------------------------------------------------------------------


//Special fish speaces definition------------------------------------------------------------------------------------------
SpecialFish::SpecialFish(Point2D position, unsigned char size, SPECIES spec, std::wstring speciesName)
    : Fish(position,size,spec), _speciesName(speciesName)
{

}

SpecialFish::SpecialFish(int x, int y, unsigned char size, SPECIES spec, std::wstring speciesName)
    : Fish(Point2D(x,y),size,spec), _speciesName(speciesName)
{

}

const std::wstring& SpecialFish::GetSpeciesName() const
{
    return _speciesName;
}
//-------------------------------------------------------------------------------------------------------------------------


//CrucianCarp fish definition------------------------------------------------------------------------------------------
CrucianCarp::CrucianCarp(Point2D position, unsigned char size) : Fish(position,size,SPECIES::Herbivorous)
{

}

CrucianCarp::CrucianCarp(int x, int y, unsigned char size) : Fish(Point2D(x,y),size,SPECIES::Herbivorous)
{

}
//-------------------------------------------------------------------------------------------------------------------------


NorthernPike::NorthernPike(Point2D position, unsigned char size) : Fish(position,size,SPECIES::Carnivorous)
{

}

NorthernPike::NorthernPike(int x, int y, unsigned char size) : Fish(Point2D(x,y),size,SPECIES::Carnivorous)
{

}


EuropeanChub::EuropeanChub(Point2D position, unsigned char size) : Fish(position,size,SPECIES::Omnivorous)
{

}

EuropeanChub::EuropeanChub(int x, int y, unsigned char size) : Fish(Point2D(x,y),size,SPECIES::Omnivorous)
{

}
