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

private slots:
    void on_actionOpen_File_triggered();
    void on_playPushButton_clicked();

private:
    bool _fileIsLoaded;
    int _xSize;
    int _ySize;
    int _fishNumber;
    int _plantsNumber;
    QVector<QLabel*> _picturelabels;
    QVector<QLabel*> _waterObjectLabels;
    std::vector<std::wstring> _fishiesName;
    std::vector<WaterObject*> _waterObjects;

    LakeSimulation* _simulation = nullptr;
    Ui::MainWindow *ui;

    //Private functions
    void drawnTheCleanWater();
    void drawnWaterElement();
    void setSpritesAndToolTipStr(WaterObject* i, QString& path, QString& toolTip);

};

#endif // MAINWINDOW_H
