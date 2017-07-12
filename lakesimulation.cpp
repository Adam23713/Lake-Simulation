#include "lakesimulation.h"
#include <limits>
#include <QDebug>

LakeSimulation::LakeSimulation(QObject *parent, unsigned int x, unsigned int y, std::vector<std::vector<WaterObject *>> *map)
    : QThread(parent), _xSize(x), _ySize(y), _gridMap(map)
{
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

        runNextStep();
        emit changeMap();

        for(unsigned int x = 0; x < _gridMap->size(); x++)
            for(unsigned int y = 0; y < _gridMap->at(x).size(); y++)
            {
                WaterObject* obj = _gridMap->at(x).at(y);
                if(obj != nullptr && obj->GetType() == WaterObjectType::FISH)
                    static_cast<Fish*>(obj)->SetMoved(false);
            }

        this->msleep(1000);
    }

}

void LakeSimulation::simulationSpeedChange(int speed)
{
    _speed = speed;
}

void LakeSimulation::continueSimulation()
{
    _wait = false;
}

void LakeSimulation::pauseTheSimulation()
{
    _pause = !_pause;
}

void LakeSimulation::runNextStep()
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
