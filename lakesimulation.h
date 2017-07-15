#ifndef LAKESIMULATION_H
#define LAKESIMULATION_H

#include <QtCore>
#include <QObject>
#include <algorithm>
#include <random>
#include "waterobject.h"
#include "waterplants.h"
#include "fishes.h"
#include "deadzone.h"


//Ez az osztály irányítja a szimuláció menetét, valamint ő a kapcsolat a szimuláció és a megjelenítő osztály között.

class LakeSimulation : public QThread
{

    Q_OBJECT

private:
    enum class SIDE {TOP, BOTTOM, RIGHT, LEFT};

    bool _dryTheLake = true;
    bool _pause = false;
    int _speed = 1000;

    //A tó széleit jelöli, valamint a sewgítségükkel optimalizálni lehet a megjelenítést
    int _deadZoneNumber;
    int _xTopBorder;
    int _yRightBorder;
    int _xBottomBorder;
    int _yLeftBorder;
    //--------------------------

    unsigned int _xSize;
    unsigned int _ySize;
    std::mt19937 _randomEngine;
    std::vector<std::vector<WaterObject*>> *_gridMap;

    //Private Functions
    int syncDry();
    void dryTheLake();
    void dryTopOrDown(SIDE side);
    void dryRightOrLeft(SIDE side);
    void moveAndEatAllFish();
    WaterObject* getPlantTarget(int i);
    WaterObject* getFishTarget(int i);
    WaterObject* getWaterObjecTarget(Fish *fish);

public:
    void setDry(bool value);
    void GetLakeBorder(int* top, int* right, int* bottom, int* left);
    void simulationSpeedChange(int speed);
    LakeSimulation() = delete;
    explicit LakeSimulation(QObject *parent, unsigned int x, unsigned int y, std::vector<std::vector<WaterObject *> > *map);
    void run();

//Jelek a MainWindow (a megjelenítő osztály) számára
signals:
    void SimulationFinish();
    void changeMap();

public slots:
    void pauseTheSimulation();
};

#endif // LAKESIMULATION_H
