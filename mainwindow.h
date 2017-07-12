#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <set>
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
    void drawnGidDone();
    void pauseTheSimulation();

private slots:
    void updateLake();
    void on_actionOpen_File_triggered();
    void on_playPushButton_clicked();

private:
    bool _paused = false;
    bool _newsimulation = true;
    bool _fileIsLoaded;
    int _xSize;
    int _ySize;
    int _fishNumber;
    int _plantsNumber;
    std::vector<std::vector<WaterObject*>> _gridMap;
    QVector<QLabel*> _picturelabels;
    QVector<QLabel*> _waterObjectLabels;
    std::vector<std::wstring> _fishiesName;
    std::vector<WaterObject*> _waterObjects;

    LakeSimulation* _simulation = nullptr;
    Ui::MainWindow *ui;

    //Private functions
    void callFactory();
    void createWaterObjectMap();
    void drawnTheCleanWater();
    void drawnWaterElement();
    void setSpritesAndToolTipStr(WaterObject* i, QString& path, QString& toolTip);

    void deleteGridMap();
};

#endif // MAINWINDOW_H
