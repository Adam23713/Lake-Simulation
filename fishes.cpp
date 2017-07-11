#include "fishes.h"
#include <random>
#include <QDebug>

//Fish Definition----------------------------------------------------------------------------------------------------
Fish::Fish(Point2D position, unsigned char size, SPECIES spec) : WaterObject(position, WaterObjectType::FISH), _size(size), _species(spec)
{

}

Fish::Fish(int x, int y, unsigned char size, SPECIES spec) : WaterObject(Point2D(x,y),WaterObjectType::FISH), _size(size), _species(spec)
{

}

void Fish::setTheMapGrid(std::vector<std::vector<WaterObject *> > &grid)
{
    _grid = grid;
    _xSize = _grid.size();
    _ySize = _grid.at(0).size();
}

void Fish::SetTarget(WaterObject *fish)
{
    _target = fish;
}

WaterObject* Fish::ShowTarget() const
{
    return _target;
}

const std::wstring& SpecialFish::GetSpeciesName() const
{
    return _speciesName;
}

SPECIES Fish::GetSpecies() const
{
    return _species;
}

unsigned char Fish::GetSize() const
{
    return _size;
}

void Fish::lookAround(WaterObject* vector[])
{
    int x = this->GetPosition().GetXPosition();
    int y = this->GetPosition().GetYPosition();

    if(x >= 1 && y >= 1)
    {
        vector[0] = _grid.at(x-1).at(y-1);
    }
    else
        vector[0] = nullptr;

    if( y >= 1)
    {
        vector[1] = _grid.at(x).at(y-1);
    }
    else
         vector[1] = nullptr;

    if( (x < _xSize - 1) && ( y >= 1 ) )
    {
        vector[2] = _grid.at(x+1).at(y-1);
    }
    else
        vector[2] = nullptr;

    if( x < _xSize - 1)
    {
        vector[3] = _grid.at(x+1).at(y);
    }
    else
        vector[3] = nullptr;

    if( (x < _xSize - 1) && (y < _ySize - 1) )
    {
        vector[4] = _grid.at(x+1).at(y+1);
    }
    else
        vector[4] = nullptr;

    if( y < _ySize - 1)
    {
        vector[5] = _grid.at(x).at(y+1);
    }
    else
        vector[5] = nullptr;


    if( (y < _ySize - 1) && ( x >= 1) )
    {
        vector[6] = _grid.at(x-1).at(y+1);
    }
    else
        vector[6] = nullptr;

    if( x >= 1)
    {
        vector[7] = _grid.at(x-1).at(y);
    }
    else
        vector[7] = nullptr;
}

Point2D Fish::lookAroundForTheFood(WaterObject* vector[])
{
    //Fish look around
    lookAround(vector);

    //If find something than eat it
    for(int i = 0; i < 8; i++)
    {
        if(vector[i] != nullptr)
        {

            if( _species == SPECIES::Carnivorous && vector[i]->GetType() == WaterObjectType::FISH )
            {
                if( _size > static_cast<Fish*>(vector[i])->_size && vector[i]->GetLive())
                    return vector[i]->GetPosition();
            }

            if( _species == SPECIES::Herbivorous && vector[i]->GetType() == WaterObjectType::PLANT && vector[i]->GetLive())
            {
                return vector[i]->GetPosition();
            }

            if( _species == SPECIES::Omnivorous && ( vector[i]->GetType() == WaterObjectType::FISH  || vector[i]->GetType() == WaterObjectType::PLANT ) && vector[i]->GetLive() )
            {
                if(vector[i]->GetType() == WaterObjectType::FISH)
                {
                    if( _size > static_cast<Fish*>(vector[i])->_size )
                    {
                        return vector[i]->GetPosition();
                    }
                }
                else
                    return vector[i]->GetPosition();
            }
        }
    }
    return Point2D(-1,-1);
}

void Fish::randomMove()
{
    const int direction = 8;
    WaterObject* vector[direction];

    lookAround(vector);

    //Search free space
    for(int i = 0; i < direction; i++)
    {
        if( (vector[i] == nullptr) || (vector[i] != nullptr && !vector[i]->GetLive()))
        {
            break;
        }
        else
            return; //No free space
    }

    //Set random engine
    std::mt19937 randomEngine;
    randomEngine.seed(time(nullptr));
    std::uniform_int_distribution<int> choose(0,7);

    //Choose a random move function
    bool succesFullStep = false;
    while( !succesFullStep )
    {
        int num = choose(randomEngine);
        succesFullStep = moveXY(vector[num], generateNewPointCoordinate( DIRECTION(num) ) );
    }
}

//Move functions****************************************************************
bool Fish::moveXY(WaterObject* object, Point2D point)
{
    if( (object == nullptr) || (object != nullptr && !object->GetLive()))
    {
        Point2D newCoordinate = Point2D(this->GetPosition().GetXPosition(),this->GetPosition().GetYPosition()) + point;

        if( (newCoordinate.GetXPosition() < 0 || newCoordinate.GetXPosition() >= _xSize) || (newCoordinate.GetYPosition() < 0 || newCoordinate.GetYPosition() >= _ySize))
            return false;

        this->SetPosition( newCoordinate );
        return true;
    }
    return false;
}

Point2D Fish::generateNewPointCoordinate(DIRECTION direction)
{
    switch(direction)
    {
    case DIRECTION::TopLeft : return Point2D(-1,-1);
    case DIRECTION::Top : return Point2D(0,-1);
    case DIRECTION::TopRight : return Point2D(1,-1);

    case DIRECTION::Right : return Point2D(1,0);

    case DIRECTION::BottomRight : return Point2D(1,1);
    case DIRECTION::Bottom : return Point2D(0,1);
    case DIRECTION::BottomLeft : return Point2D(-1,1);

    case DIRECTION::Left : return Point2D(-1,0);

    default : return Point2D(0,0);
    }
}

//*******************************************************************************

void Fish::Move()
{
    if(_target == nullptr)
    {
        randomMove();
        return;
    }

    const int direction = 8;
    WaterObject* vector[direction];

    Point2D targetCoordinate = _target->GetPosition();
    int targetX = targetCoordinate.GetXPosition();
    int targetY = targetCoordinate.GetYPosition();

    int myX = this->GetPosition().GetXPosition();
    int myY = this->GetPosition().GetYPosition();


    Point2D p = lookAroundForTheFood(vector);
    if(p.GetXPosition() != -1 && p.GetYPosition() != -1)
    {
        Eat( _grid.at(p.GetXPosition()).at(p.GetYPosition()) );
        return;
    }

    //Search free space------------------------------------------------------------------------
    for(int i = 0; i < direction; i++)
        if( (vector[i] == nullptr) || (vector[i] != nullptr && !vector[i]->GetLive()))
            break;
        else
            return; //No free space
    //----------------------------------------------------------------------------------------

    //Top left corner
    bool succesFullStep;
    if( ( targetX < myX ) && ( targetY < myY ) )
    {
        succesFullStep = moveXY(vector[int(DIRECTION::TopLeft)],generateNewPointCoordinate(DIRECTION::TopLeft));
        if(succesFullStep) return;
    }
    //Top
    if( ( targetX == myX ) && ( targetY < myY ) )
    {
        succesFullStep = moveXY(vector[int(DIRECTION::Top)],generateNewPointCoordinate(DIRECTION::Top));
        if(succesFullStep) return;
    }
    //Top right corner
    if( ( targetX > myX ) && ( targetY < myY ) )
    {
        succesFullStep = moveXY(vector[int(DIRECTION::TopRight)],generateNewPointCoordinate(DIRECTION::TopRight));
        if(succesFullStep) return;
    }
    //Right
    if( ( targetX > myX ) && ( targetY == myY ) )
    {
        succesFullStep = moveXY(vector[int(DIRECTION::Right)],generateNewPointCoordinate(DIRECTION::Right));
        if(succesFullStep) return;
    }
    //Bottom Right
    if( ( targetX > myX ) && ( targetY > myY ) )
    {
        succesFullStep = moveXY(vector[int(DIRECTION::BottomRight)],generateNewPointCoordinate(DIRECTION::BottomRight));
        if(succesFullStep) return;
    }
    //Bottom
    if( ( targetX == myX ) && ( targetY > myY ) )
    {
        succesFullStep = moveXY(vector[int(DIRECTION::Bottom)],generateNewPointCoordinate(DIRECTION::Bottom));
        if(succesFullStep) return;
    }
    //BottomLeft
    if( ( targetX < myX ) && ( targetY > myY ) )
    {
        succesFullStep = moveXY(vector[int(DIRECTION::BottomLeft)],generateNewPointCoordinate(DIRECTION::BottomLeft));
        if(succesFullStep) return;
    }
    //Left
    if( ( targetX < myX ) && ( targetY == myY ) )
    {
        succesFullStep = moveXY(vector[int(DIRECTION::Left)],generateNewPointCoordinate(DIRECTION::Left));
        if(succesFullStep) return;
    }

}

void Fish::Eat(WaterObject* obj)
{
    obj->SetLive(false);
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


EuropeanChub::EuropeanChub(Point2D position, unsigned char size) : Fish(position,size,SPECIES::Herbivorous)
{

}

EuropeanChub::EuropeanChub(int x, int y, unsigned char size) : Fish(Point2D(x,y),size,SPECIES::Herbivorous)
{

}
