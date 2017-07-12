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
    bool _pause = false;
    int _speed = 1000;
    unsigned int _xSize;
    unsigned int _ySize;
    //std::vector<WaterPlant*> _plantsVector;
    std::vector<std::vector<WaterObject*>> *_gridMap;
    //std::vector<Fish*> _fishVector;

    //Private Functions
    void runNextStep();
    WaterObject* getPlantTarget(int i);
    WaterObject* getFishTarget(int i);
    WaterObject* getWaterObjecTarget(Fish *fish);

public:
    void simulationSpeedChange(int speed);
    LakeSimulation() = delete;
    explicit LakeSimulation(QObject *parent, unsigned int x, unsigned int y, std::vector<std::vector<WaterObject *> > *map);
    void run();

signals:
    void stoped();
    void changeMap();

public slots:
    void pauseTheSimulation();
};

#endif // LAKESIMULATION_H
