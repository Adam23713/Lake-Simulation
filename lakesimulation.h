#ifndef LAKESIMULATION_H
#define LAKESIMULATION_H

#include <QtCore>
#include <QObject>
#include <algorithm>
#include "waterobject.h"
#include "waterplants.h"
#include "fishes.h"

class LakeSimulation : public QThread
{

    Q_OBJECT

private:
    unsigned int _xSize;
    unsigned int _ySize;
    std::vector<WaterPlant*> _plantsVector;
    std::vector<std::vector<WaterObject*>> _gridMap;
    std::vector<Fish*> _fishVector;

    //Private Functions
    void runNextStep();
    WaterObject* getPlantTarget(int i);
    WaterObject* getFishTarget(int i);
    WaterObject* getWaterObjecTarget(int i);

public:
    LakeSimulation() = delete;
    explicit LakeSimulation(QObject *parent, unsigned int x, unsigned int y, std::vector<WaterObject*>& vec, std::vector<std::vector<WaterObject*>>& map);
    void run();

signals:
    void changeMap();

public slots:

};

#endif // LAKESIMULATION_H
