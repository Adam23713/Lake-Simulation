#ifndef LAKESIMULATION_H
#define LAKESIMULATION_H

#include <QtCore>
#include "waterobject.h"
#include "waterplants.h"
#include "fishes.h"

class LakeSimulation : public QThread
{
public:
    LakeSimulation() = delete;
    LakeSimulation(unsigned int x, unsigned int y, std::vector<WaterObject*>& vec);
    void run();
};

#endif // LAKESIMULATION_H
