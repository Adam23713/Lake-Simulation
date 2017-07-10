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

Fish *LakeSimulation::getTarget(int i)
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
        return _fishVector.at(index);
        //qDebug() << fish->GetPosition().GetXPosition() << fish->GetPosition().GetYPosition() << "\n" <<_fishVector.at(index)->GetPosition().GetXPosition() << _fishVector.at(index)->GetPosition().GetYPosition();
        //qDebug() << "------------------------";
    }
    return nullptr;
}

void LakeSimulation::run()
{
    if(_fishVector.size() == 0) return;

    //Add target every fish----------------------------------
    for(int i = 0; i < (int)_fishVector.size(); i++)
    {
        if(_fishVector.at(i)->GetSize() == 1) continue;
        _fishVector.at(i)->SetTarget(getTarget(i));
    }
    //--------------------------------------------------------



}
