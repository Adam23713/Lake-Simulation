#include "lakesimulation.h"
#include <limits>
#include <QDebug>

LakeSimulation::LakeSimulation(unsigned int x, unsigned int y, std::vector<WaterObject *> &vec) : _xSize(x), _ySize(y)
{
    for(auto i : vec)
    {
        if(i->GetType() == WaterObjectType::FISH)
        {
            _fishVector.push_back(static_cast<Fish*>(i));
        }
        else if(i->GetType() == WaterObjectType::PLANT)
        {
            _plantsVector.push_back(static_cast<WaterPlant*>(i));
        }
    }

    std::sort(_fishVector.begin(), _fishVector.end(), [](const Fish* fish1, const Fish* fish2){ return fish1->GetSize() < fish2->GetSize(); } );

    for(int i = 0; i < (int)_fishVector.size(); i++)
        qDebug() << _fishVector.at(i)->GetSize();

}

WaterObject *LakeSimulation::getPlantTarget(int i)
{
    Fish* fish = _fishVector.at(i);
    float nearestPlant = std::numeric_limits<float>::max();
    WaterPlant *plantTarget = nullptr;
    for(unsigned int plantIndex = 0; plantIndex < _plantsVector.size(); plantIndex++)
    {
        float plantDistance = Point2D::GetDistance(fish->GetPosition(),_plantsVector.at(plantIndex)->GetPosition());
        if(nearestPlant > plantDistance)
        {
            nearestPlant = plantDistance;
            plantTarget = _plantsVector.at(plantIndex);
        }
    }
    return plantTarget;
}

WaterObject *LakeSimulation::getFishTarget(int i)
{
    Fish* fish = _fishVector.at(i);
    if(i > 0)
    {
        int index = i - 1;
        float minimumDistance = std::numeric_limits<float>::max();
        for(int j = i - 1; j >= 0; j--)
        {
            if(fish->GetSize() == _fishVector.at(j)->GetSize()) continue;

            float distance = Point2D::GetDistance(fish->GetPosition(),_fishVector.at(j)->GetPosition());
            if( distance < minimumDistance )
            {
                index = j;
                minimumDistance = distance;
            }
        }
        if(fish->GetSize() == _fishVector.at(index)->GetSize())
            return nullptr;
        else
            return _fishVector.at(index);
    }
    return nullptr;
}

WaterObject *LakeSimulation::getWaterObjecTarget(int i)
{
    //Herbivorous - Növényevő
    if(_fishVector.at(i)->GetSpecies() == SPECIES::Herbivorous)
        return getPlantTarget(i);

    //Carnivorous - Húsevő
    if(_fishVector.at(i)->GetSpecies() == SPECIES::Carnivorous)
        return getFishTarget(i);

    //Omnivorous - Mindenevő
    if(_fishVector.at(i)->GetSpecies() == SPECIES::Omnivorous)
    {
        WaterObject* obj1 = getPlantTarget(i);
        if(_fishVector.at(i)->GetSize() == 1)
            return obj1;

        WaterObject* obj2 = getFishTarget(i);

        float dist1 = Point2D::GetDistance(_fishVector.at(i)->GetPosition(), obj1->GetPosition());
        float dist2 = Point2D::GetDistance(_fishVector.at(i)->GetPosition(), obj2->GetPosition());

        //First target: Plant
        if( dist1 <= dist2 )
            return obj1;
        else
            return obj2;
    }
    return nullptr;
}

void LakeSimulation::run()
{
    if(_fishVector.size() == 0) return;

    //Add target every fish----------------------------------
    for(int i = 0; i < (int)_fishVector.size(); i++)
        _fishVector.at(i)->SetTarget(getWaterObjecTarget(i));
    //--------------------------------------------------------

    /*for(int i = 0; i < (int)_fishVector.size(); i++)
    {
        Fish* fish = _fishVector.at(i);
        if(fish->ShowTarget() != nullptr)
        {
            qDebug() << fish->GetPosition().GetXPosition() << fish->GetPosition().GetYPosition() << "\n" << fish->ShowTarget()->GetPosition().GetXPosition() << fish->ShowTarget()->GetPosition().GetYPosition();
            qDebug() << "------------------------";
        }
        else
        {
            qDebug() << fish->GetPosition().GetXPosition() << fish->GetPosition().GetYPosition() << "\nNo target";
        }
    }*/

    //Move every fish


}
