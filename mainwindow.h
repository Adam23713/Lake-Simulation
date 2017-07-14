#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <fstream>
#include <set>
#include "parametersdialog.h"
#include "deadzone.h"
#include "lakesimulation.h"
#include "factories.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void pauseTheSimulation();

private slots:
    void SimulationFinish();
    void stopedSimAndExitApp();
    void updateLake();
    void on_actionOpen_File_triggered();
    void on_playPushButton_clicked();
    void on_forwardPushButton_clicked();
    void on_rewindPushButton_clicked();
    void on_actionExit_triggered();

    void on_actionChange_parameters_triggered();

private:
    int _oldTopBorder;
    int _oldRightBorder;
    int _oldBottomBorder;
    int _oldLeftBorder;
    bool _dryTheLake = true;
    bool _exit = false;
    bool _paused = false;
    bool _newsimulation = true;
    bool _fileIsLoaded;
    bool _simulationFinished = false;
    int _simulationSpeed = 1000;
    int _xSize;
    int _ySize;
    int _fishNumber;
    int _plantsNumber;
    std::vector<std::vector<WaterObject*>> _gridMap;
    QVector<QLabel*> _picturelabels;
    QVector<QLabel*> _waterObjectLabels;
    std::vector<std::wstring> _fishiesName;
    std::vector<WaterObject*> _waterObjects;

    ParametersDialog *_settingsDialog = nullptr;
    LakeSimulation *_simulation = nullptr;
    Ui::MainWindow *ui;

    //Private functions
    void resetBorders();
    void drawnDeadZone(unsigned int x, unsigned int y);
    void closeEvent (QCloseEvent *event) override;
    void cleanSimulationObject();
    void callFactory();
    void createWaterObjectMap();
    void drawnTheCleanWater();
    void drawnWaterElement();
    void setSpritesAndToolTipStr(WaterObject* i, QString& path, QString& toolTip);
    void deleteGridMap();
    void resetVectorsAndData();
    void inicializeTheLake();
};

#endif // MAINWINDOW_H
