#ifndef LAKESIMULATION_H
#define LAKESIMULATION_H

#include <QtCore>
#include <algorithm>
#include "waterobject.h"
#include "waterplants.h"
#include "fishes.h"

class LakeSimulation : public QThread
{
private:
    unsigned int _xSize;
    unsigned int _ySize;
    std::vector<WaterPlant*> _plantsVector;
    std::vector<Fish*> _fishVector;

public:
    LakeSimulation() = delete;
    LakeSimulation(unsigned int x, unsigned int y, std::vector<WaterObject*>& vec);
    void run();

private:
    Fish* getTarget(int i);
};

#endif // LAKESIMULATION_H
