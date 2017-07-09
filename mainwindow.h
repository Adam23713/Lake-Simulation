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
    QVector<QLabel*> _picturelabels;
    std::set<std::wstring> _fishiesName;

    LakeSimulation* _simulation;
    Ui::MainWindow *ui;

    //Private functions
    void drawnTheWatertoTheLake();

};

#endif // MAINWINDOW_H
