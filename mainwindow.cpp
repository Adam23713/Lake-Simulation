#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    _fileIsLoaded = false;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    for(auto i : _picturelabels) delete i;
    delete ui;
}

//Open file, read it and drawn the lake
void MainWindow::on_actionOpen_File_triggered()
{
    QStringList xySize;
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open file"), tr("."), tr("*.txt"));
    QFile inputFile(fileName);

    if( inputFile.open(QIODevice::ReadOnly) )
    {
        QTextStream in(&inputFile);

        QString line = in.readLine(); //Read the lake size
        xySize = line.split(";");
        _xSize = xySize.at(0).toInt();
        _ySize = xySize.at(1).toInt();

        while(!in.atEnd())
        {
            line = in.readLine();
            _fishiesName.insert(line.toStdWString());
        }
        inputFile.close();

        //Drawn the lake and enabled the start button
        if(_fileIsLoaded)
        {
            for(auto i : _picturelabels)
                delete i;
            _picturelabels.resize(0);
        }
        _fileIsLoaded = true;
        drawnTheWatertoTheLake();
        ui->playPushButton->setEnabled(true);
        //********************************************
    }
    else
        QMessageBox::critical(this,"File error","I can't open the file!");
}

//Start/pause the simulation
void MainWindow::on_playPushButton_clicked()
{
    _simulation = new LakeSimulation(_xSize, _ySize);
}

void MainWindow::drawnTheWatertoTheLake()
{
    QPixmap image(":/images/images/water.png");
    ui->lakeGrid->setSpacing(0);
    ui->lakeGrid->setAlignment(Qt::AlignCenter);

    for(int i = 0; i < _xSize; i++)
    {
        for(int j = 0; j < _ySize; j++)
        {
            QLabel* label = new QLabel(this);
            label->setFixedSize(image.size());
            label->setPixmap(image);
            label->show();
            ui->lakeGrid->addWidget(label,i,j);
            _picturelabels.push_back(label);
        }
    }
}
