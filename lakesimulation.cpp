#include "lakesimulation.h"
#include <limits>

LakeSimulation::LakeSimulation(QObject *parent, unsigned int x, unsigned int y, std::vector<std::vector<WaterObject *>> *map)
    : QThread(parent), _xSize(x), _ySize(y), _gridMap(map)
{
    _randomEngine.seed(time(nullptr));
    //Due to dehydration--------
    _deadZoneNumber = _xSize * _ySize;
    _xTopBorder = 0;
    _xBottomBorder = _xSize - 1;
    _yRightBorder = _ySize - 1;
    _yLeftBorder = 0;
    //----------------------------
}

WaterObject *LakeSimulation::getWaterObjecTarget(Fish* fish)
{
    WaterObject* target = nullptr;
    float nearestTarget = std::numeric_limits<float>::max();
    for(unsigned int x = 0; x < _gridMap->size(); x++)
    {
        for(unsigned int y = 0; y < _gridMap->at(x).size(); y++)
        {
            WaterObject* obj = _gridMap->at(x).at(y);

            if( obj == nullptr) continue;
            if(obj == static_cast<WaterObject*>(fish)) continue;
            if(fish->GetSpecies() == SPECIES::Herbivorous && (obj->GetType() != WaterObjectType::PLANT)) continue;
            if(fish->GetSpecies() == SPECIES::Carnivorous && (obj->GetType() == WaterObjectType::PLANT)) continue;

            if(obj->GetType() == WaterObjectType::FISH)
            {
                if( fish->GetSize() <= static_cast<Fish*>(obj)->GetSize() )
                    continue;
            }

            float distance = Point2D::GetDistance(fish->GetPosition(),obj->GetPosition());
            if(nearestTarget > distance)
            {
                nearestTarget = distance;
                target = obj;
            }
        }
    }
    return target;
}

void LakeSimulation::setDry(bool value)
{
    _dryTheLake = value;
}

void LakeSimulation::GetLakeBorder(int *top, int *right, int *bottom, int *left)
{
    *top = _xTopBorder;
    *right = _yRightBorder;
    *bottom = _xBottomBorder;
    *left = _yLeftBorder;
}

void LakeSimulation::run()
{

    for(unsigned int x = 0; x < _gridMap->size(); x++)
    {
        for(unsigned int y = 0; y < _gridMap->at(x).size(); y++)
        {
            WaterObject* obj = _gridMap->at(x).at(y);
            if(obj != nullptr && obj->GetType() == WaterObjectType::FISH)
               static_cast<Fish*>(obj)->setTheMapGrid(_gridMap);
        }
    }

    while(!_pause)
    {
        QMutex mutex;
        mutex.lock();
        if(_pause == true) break;
        mutex.unlock();

        Fish::currentTime++;
        moveAndEatAllFish();

        if(_dryTheLake)
        {
            if(_deadZoneNumber > 0)
            {
                dryTheLake();
            }
            else
            {
                if( (_deadZoneNumber = syncDry()) == 0 )
                {
                    emit changeMap();
                    emit SimulationFinish();
                    break;
                }
            }
            _deadZoneNumber--;
        }
        emit changeMap();

        //All fish will get the chance to move
        for(unsigned int x = 0; x < _gridMap->size(); x++)
            for(unsigned int y = 0; y < _gridMap->at(x).size(); y++)
            {
                WaterObject* obj = _gridMap->at(x).at(y);
                if(obj != nullptr && obj->GetType() == WaterObjectType::FISH)
                    static_cast<Fish*>(obj)->SetMoved(false);
            }

        this->msleep(_speed);
    }

}

void LakeSimulation::simulationSpeedChange(int speed)
{
    _speed = speed;
}

void LakeSimulation::pauseTheSimulation()
{
    _pause = !_pause;
}

void LakeSimulation::dryTopOrDown(LakeSimulation::SIDE side)
{
    std::uniform_int_distribution<int> probability(0,4); // 1/5 probability to dry

    int i,j;
    if(side == SIDE::TOP)
    {
        i = 0;
        j = _xTopBorder;
    }
    if(side == SIDE::BOTTOM)
    {
        i = 0;
        j = _xBottomBorder;
    }

    bool itHappened = false;
    while(!itHappened)
    {
        for(unsigned int x = i; x < _gridMap->at(j).size(); x++)
        {
            if( probability(_randomEngine) != 4) continue;

            itHappened = true;
            if( _gridMap->at(j).at(x) == nullptr )
            {
                _gridMap->at(j).at(x) = new DeadZone(j,x,WaterObjectType::DEADZONE);
                break;
            }
            if(_gridMap->at(j).at(x) != nullptr)
            {
                WaterObjectType type = _gridMap->at(j).at(x)->GetType();
                if( type == WaterObjectType::DEADZONE)
                {
                    itHappened = false;
                    continue;
                }
                if(type == WaterObjectType::FISH)
                {
                    _gridMap->at(j).at(x) = new DeadZone(j,x,WaterObjectType::FISH);
                    break;
                }
                if(type == WaterObjectType::PLANT)
                {
                    _gridMap->at(j).at(x) = new DeadZone(j,x,WaterObjectType::PLANT);
                    break;
                }
            }
            break;
        }
        bool freeSpace = false;
        for(unsigned int x = i; x < _gridMap->at(j).size(); x++)
        {
            WaterObject* obj = _gridMap->at(j).at(x);
            if( obj == nullptr || obj->GetType() != WaterObjectType::DEADZONE)
            {
                freeSpace = true;
                break;
            }
        }
        if(freeSpace == false)
        {
            if(side == SIDE::TOP)
                _xTopBorder++; //Ellenőrzés még kell
            if(side == SIDE::BOTTOM)
                _xBottomBorder--;
            break;
        }
    }//while end
}

void LakeSimulation::dryRightOrLeft(LakeSimulation::SIDE side)
{
    std::uniform_int_distribution<int> probability(0,4); // 1/5 probability to dry

    int i,j;
    if(side == SIDE::RIGHT)
    {
        i = 0;
        j = _yRightBorder;
    }
    if(side == SIDE::LEFT)
    {
        i = 0;
        j = _yLeftBorder;
    }

    bool itHappened = false;
    while(!itHappened)
    {
        for(unsigned int y = i; y < _gridMap->size(); y++)
        {
            if( probability(_randomEngine) != 4) continue;

            itHappened = true;
            if( _gridMap->at(y).at(j) == nullptr )
            {
                _gridMap->at(y).at(j) = new DeadZone(y,j,WaterObjectType::DEADZONE);
                break;
            }
            if(_gridMap->at(y).at(j) != nullptr)
            {
                WaterObjectType type = _gridMap->at(y).at(j)->GetType();
                if( type == WaterObjectType::DEADZONE)
                {
                    itHappened = false;
                    continue;
                }
                if(type == WaterObjectType::FISH)
                {
                    _gridMap->at(y).at(j) = new DeadZone(y,j,WaterObjectType::FISH);
                    break;
                }
                if(type == WaterObjectType::PLANT)
                {
                    _gridMap->at(y).at(j) = new DeadZone(y,j,WaterObjectType::PLANT);
                    break;
                }
            }
            break;
        }
        bool freeSpace = false;
        for(unsigned int y = i; y < _gridMap->size(); y++)
        {
            WaterObject* obj = _gridMap->at(y).at(j);
            if( obj == nullptr || obj->GetType() != WaterObjectType::DEADZONE)
            {
                freeSpace = true;
                break;
            }
        }
        if(freeSpace == false)
        {
            if(side == SIDE::RIGHT)
                _yRightBorder--; //Ellenőrzés még kell
            if(side == SIDE::LEFT)
                _yLeftBorder++;
            break;
        }
    }//while end

}

int LakeSimulation::syncDry()
{
    std::vector<WaterObject*> theRestDeadTone;
    for(unsigned int x = 0; x < _gridMap->size(); x++)
    {
        for(unsigned int y = 0; y < _gridMap->at(x).size(); y++)
        {
            if( _gridMap->at(x).at(y) == nullptr)
            {
                _gridMap->at(x).at(y) = new DeadZone(x,y,WaterObjectType::DEADZONE);
                return 1;
            }
             else if( _gridMap->at(x).at(y)->GetType() != WaterObjectType::DEADZONE )
            {
                theRestDeadTone.push_back(_gridMap->at(x).at(y));
            }
        }
    }

    if(theRestDeadTone.size() == 0) return 0;

    //Choose random field
    int num;
    std::uniform_int_distribution<int> choosen(0,theRestDeadTone.size()-1);

    if(theRestDeadTone.size() == 1)
        num = 0;
    else
        num = choosen(_randomEngine);

    int x = theRestDeadTone.at(num)->GetPosition().GetXPosition();
    int y = theRestDeadTone.at(num)->GetPosition().GetYPosition();
    WaterObjectType type = theRestDeadTone.at(num)->GetType();

    if(type == WaterObjectType::FISH)
        _gridMap->at(x).at(y) = new DeadZone(x,y,WaterObjectType::FISH);
    if(type == WaterObjectType::PLANT)
        _gridMap->at(x).at(y) = new DeadZone(x,y,WaterObjectType::PLANT);

    return theRestDeadTone.size() - 1;
}

void LakeSimulation::dryTheLake()
{
    std::uniform_int_distribution<int> chooseASide(int(SIDE::TOP), int(SIDE::LEFT));
    SIDE choosenSide = SIDE(chooseASide(_randomEngine));

    if(choosenSide == SIDE::TOP || choosenSide == SIDE::BOTTOM)
        dryTopOrDown(choosenSide);
    else if(choosenSide == SIDE::RIGHT || choosenSide == SIDE::LEFT)
            dryRightOrLeft(choosenSide);

}

void LakeSimulation::moveAndEatAllFish()
{
    //Add target every fish----------------------------------
    for(unsigned int x = 0; x < _gridMap->size(); x++)
    {
        for(unsigned int y = 0; y < _gridMap->at(x).size(); y++)
        {
            WaterObject* obj = _gridMap->at(x).at(y);
            if(obj != nullptr && obj->GetType() == WaterObjectType::FISH)
               static_cast<Fish*>(obj)->SetTarget(getWaterObjecTarget(static_cast<Fish*>(obj)));
        }
    }
    //--------------------------------------------------------

    //Move every fish
    for(unsigned int x = 0; x < _gridMap->size(); x++)
    {
        for(unsigned int y = 0; y < _gridMap->at(x).size(); y++)
        {
            WaterObject* obj = _gridMap->at(x).at(y);
            if(obj != nullptr && obj->GetType() == WaterObjectType::FISH)
            {
                Fish* fish = static_cast<Fish*>(obj);
                if(fish->Moved() == false)
                    fish->Move();
            }
        }
    }

}
