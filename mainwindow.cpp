#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <typeinfo>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    _fileIsLoaded = false;
    _xSize = 19;
    _ySize = 30;
    _fishNumber = 100;
    _plantsNumber = 30;
    ui->setupUi(this);
}

void MainWindow::deleteGridMap()
{
    for(unsigned int i = 0; i < _gridMap.size(); i++)
        for(unsigned int j = 0; j < _gridMap.at(i).size(); j++)
            if(_gridMap.at(i).at(j) != nullptr)
                delete _gridMap.at(i).at(j);
    _gridMap.resize(0);
    resetBorders();
}

MainWindow::~MainWindow()
{
    for(auto i : _picturelabels) delete i;
    deleteGridMap();
    for(auto i : _waterObjectLabels) delete i;
    cleanSimulationObject();
    delete ui;
}

void MainWindow::SimulationFinish()
{
    _simulationSpeed = 1000;
    ui->playPushButton->setEnabled(false);
    ui->forwardPushButton->setEnabled(false);
    ui->rewindPushButton->setEnabled(false);
    ui->playPushButton->setText("Play");
    ui->playPushButton->setIcon(QIcon(":/images/images/play.png"));
    _newsimulation = true;
    _paused = false;
    _simulationFinished = true;
}

void MainWindow::cleanSimulationObject()
{
    if(_simulation != nullptr)
    {
        _simulationSpeed = 1000;
        disconnect(_simulation, SIGNAL(changeMap()), this, SLOT(updateLake()));
        disconnect(this,SIGNAL(pauseTheSimulation()), _simulation, SLOT(pauseTheSimulation()));
        disconnect(_simulation,SIGNAL(finished()),this,SLOT(stopedSimAndExitApp()));
        disconnect(_simulation,SIGNAL(SimulationFinish()),this,SLOT(SimulationFinish()));
        delete _simulation;
    }
    _simulation = nullptr;
}

void MainWindow::resetBorders()
{
    //Due to dehydration--------
    _oldTopBorder = 0;
    _oldBottomBorder = _xSize - 1;
    _oldRightBorder = _ySize - 1;
    _oldLeftBorder = 0;
    //----------------------------
}

void MainWindow::resetVectorsAndData()
{
    if(_simulation != nullptr)
    {
        cleanSimulationObject(); //Delete _simulation variable and dissconect the signal - slot functions
        _newsimulation = true;
        _paused = false;
    }

    for(auto i : _picturelabels) delete i;
    deleteGridMap();
    for(auto i : _waterObjectLabels) delete i;

    //Resize the vectors
    _picturelabels.resize(0);
    _fishiesName.resize(0);
    _waterObjects.resize(0);
    _waterObjectLabels.resize(0);
}

void MainWindow::inicializeTheLake()
{
    resetBorders();
    drawnTheCleanWater();
    callFactory();
    createWaterObjectMap();
    drawnWaterElement();
    ui->playPushButton->setEnabled(true);
}

//Open file, read it and drawn the lake
void MainWindow::on_actionOpen_File_triggered()
{
    if(_simulation != nullptr)
    {
        emit pauseTheSimulation();
        SimulationFinish();
    }

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

        //Read and add new fish species
        std::set<QString> names;
        while(!in.atEnd())
        {
            line = in.readLine();
            names.insert(line);
        }
        inputFile.close();

        //Delete old Object and datas------------------------------------------------
        if(_fileIsLoaded)
        {
            resetVectorsAndData();
        }
        _fileIsLoaded = true;
        //-----------------------------------------------------------------------------

        //Convert std::set to std::vector
        for(auto i : names)
            _fishiesName.push_back(i.toStdWString());

        //Drawn the lake and enabled the start button
        _dryTheLake = true;
        inicializeTheLake();
        //********************************************
    }
    else
        QMessageBox::critical(this,"File error","I can't open the file!");
}

//Start/pause the simulation
void MainWindow::on_playPushButton_clicked()
{
    if(_newsimulation == false)
    {
        emit pauseTheSimulation();
        _paused = !_paused;
        if(_paused)
        {
            ui->playPushButton->setText("Play");
            ui->playPushButton->setIcon(QIcon(":/images/images/play.png"));
            ui->forwardPushButton->setEnabled(false);
            ui->rewindPushButton->setEnabled(false);
        }
        else
        {
            ui->playPushButton->setText("Pause");
            ui->playPushButton->setIcon(QIcon(":/images/images/pause.png"));

            if(_simulationSpeed > 200)
                ui->forwardPushButton->setEnabled(true);

            if(_simulationSpeed  < 2000)
                ui->rewindPushButton->setEnabled(true);
            _simulation->start();
        }
        return;
    }
    else
    {
        //Create the simulation controller
        _simulationFinished = false;
        _simulation = new LakeSimulation(this, _xSize, _ySize, &_gridMap);
        _simulation->setDry(_dryTheLake);
        connect(_simulation, SIGNAL(changeMap()), this, SLOT(updateLake()));
        connect(this,SIGNAL(pauseTheSimulation()), _simulation, SLOT(pauseTheSimulation()));
        connect(_simulation,SIGNAL(finished()),this,SLOT(stopedSimAndExitApp()));
        connect(_simulation,SIGNAL(SimulationFinish()),this,SLOT(SimulationFinish()));

        _simulation->start();

        _newsimulation = false;
        ui->playPushButton->setText("Pause");
        ui->playPushButton->setIcon(QIcon(":/images/images/pause.png"));
        ui->forwardPushButton->setEnabled(true);
        ui->rewindPushButton->setEnabled(true);
    }
}

void MainWindow::callFactory()
{
    //Call Random Factory
    RandomWaterObjectFactory *factory = new RandomWaterObjectFactory(_fishiesName,_xSize,_ySize,_fishNumber,_plantsNumber);
    _waterObjects = factory->makeWaterObjectVector();
    if(_waterObjects.size() == 0)
    {
        QMessageBox::warning(this,"Too many object","The lake size is smaller than the number of fish and the number of plant.\nLake size: " + QString::number(_xSize*_ySize)+ "\nNumber of fish: " + QString::number(_fishNumber) + "\nNumber of plant: " + QString::number(_plantsNumber));
        return;
    }
}

void MainWindow::drawnTheCleanWater()
{
    //Drawn the clean lake------------------------------------------------------------------------------
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
    //------------------------------------------------------------------------------------------------
}

void MainWindow::createWaterObjectMap()
{
    if(_gridMap.size())
    {
        deleteGridMap();
    }
    _gridMap.resize(0);

    resetBorders();
    //Create the lake grid------------------------------------------------------------------------------------------------
    for(int i = 0; i < _xSize; i++)
    {
        std::vector<WaterObject*> vec;
        _gridMap.push_back(vec);
        for(int j = 0; j < _ySize; j++)
            (_gridMap.at(i)).push_back(nullptr);
    }
    for(unsigned int i = 0; i < _waterObjects.size(); i++)
        (_gridMap.at( _waterObjects.at(i)->GetPosition().GetXPosition() )).at( _waterObjects.at(i)->GetPosition().GetYPosition() ) = _waterObjects.at(i);
    //-----------------------------------------------------------------------------------------------------------------------
}

void MainWindow::drawnDeadZone(unsigned int x, unsigned int y)
{
    QString toolTip;
    QString path;
    DeadZone* zone = static_cast<DeadZone*>(_gridMap.at(x).at(y));
    if( zone->WhatIsOnIt() == WaterObjectType::FISH)
    {
        path=":/images/images/deadzoneFish.png";
        toolTip ="Dead fish";
    }
    else if(zone->WhatIsOnIt() == WaterObjectType::PLANT)
    {
        path=":/images/images/deadzonePlant.png";
        toolTip ="Dead plant";
    }
    else
    {
        path=":/images/images/dry.png";
        toolTip ="Land";
    }

    zone->setDrawnIt(true);
    int coordinates = (x * _gridMap.at(x).size()) + (y);
    _picturelabels.at(coordinates)->setToolTip(toolTip);
    _picturelabels.at(coordinates)->setPixmap(QPixmap(path));
}

void MainWindow::drawnWaterElement()
{
    unsigned int yBegin, yEnd;
    int top, right, bottom, left;

    yBegin = _oldTopBorder;
    yEnd = _oldBottomBorder;

    //Drawn Water elements
    for(unsigned int x = yBegin; x < yEnd+1; x++)
    {
        for(unsigned int y = 0; y < _gridMap.at(x).size(); y++)
        {
            if(_gridMap.at(x).at(y) == nullptr) continue;

            if(_gridMap.at(x).at(y)->GetType() == WaterObjectType::DEADZONE)
            {
                if( static_cast<DeadZone*>(_gridMap.at(x).at(y))->drawndIt() == false )
                    drawnDeadZone(x,y);
                else
                    continue;
            }

            QString path;
            QString toolTip;
            setSpritesAndToolTipStr(_gridMap.at(x).at(y),path,toolTip);

            //Set labels
            Point2D point = _gridMap.at(x).at(y)->GetPosition();
            QPixmap image(path);
            QLabel* label = new QLabel(this);

            label->setToolTip(toolTip);
            label->setPixmap(image);
            label->show();
            ui->lakeGrid->addWidget(label,point.GetXPosition(),point.GetYPosition());
            _waterObjectLabels.push_back(label);
        }
    }
    if(_simulation !=nullptr)
    {

        _simulation->GetLakeBorder(&top, &right, &bottom, &left);
        if(_oldTopBorder < top) _oldTopBorder = top;
        if(_oldBottomBorder > bottom) _oldBottomBorder = bottom;
    }
    ui->lakeGrid->update();
}

void MainWindow::setSpritesAndToolTipStr(WaterObject* i, QString& path, QString& toolTip)
{
    WaterObjectType var = i->GetType();
    Point2D point = i->GetPosition();

    if( var == WaterObjectType::PLANT)
    {
        path=":/images/images/plant.png";
        toolTip = "Type: Plant\nName: Seaweed\nCoordinates: " + QString::number(point.GetXPosition()) + ";" + QString::number(point.GetYPosition());
    }

    if( var == WaterObjectType::FISH)
    {
        Fish* fish = static_cast<Fish*>(i);
        SPECIES value = fish->GetSpecies();
        QString strSize = "\nSize: " + QString::number( fish->GetSize() );
        switch(value)
        {
            case SPECIES::Carnivorous:
                path = ":/images/images/Carnivorous.png";
                toolTip = "Type: Fish\nEat: Meats\nCoordinates: " + QString::number(point.GetXPosition()) + ";" + QString::number(point.GetYPosition()) + strSize;
                break;
            case SPECIES::Herbivorous:
                path = ":/images/images/Herbivorous.png";
                toolTip = "Type: Fish\nEat: Plants\nCoordinates: " + QString::number(point.GetXPosition()) + ";" + QString::number(point.GetYPosition()) + strSize;
                break;
            case  SPECIES::Omnivorous:
                QString fishName = QString::fromWCharArray( (static_cast<SpecialFish*>(fish))->GetSpeciesName().c_str() );
                path = ":/images/images/Omnivorous.png";
                toolTip = "Type: Fish\nName: " + fishName + "\nEat: Meats and plants\nCoordinates: " + QString::number(point.GetXPosition()) + ";" + QString::number(point.GetYPosition()) + strSize;
                break;
          }//switch end
    }
}

void MainWindow::updateLake()
{
   for(auto i : _waterObjectLabels)
   {
       ui->lakeGrid->removeWidget(i);
       delete i;
   }
   _waterObjectLabels.resize(0);
   drawnWaterElement();
}

//Speed up
void MainWindow::on_forwardPushButton_clicked()
{
    _paused = true;
    emit pauseTheSimulation();
    if( ui->rewindPushButton->isEnabled() == false )
        ui->rewindPushButton->setEnabled(true);

   _simulationSpeed -= 200;
   _simulation->simulationSpeedChange(_simulationSpeed);
   on_playPushButton_clicked();

   //Maximum speed 200ms
   if(_simulationSpeed == 200)
       ui->forwardPushButton->setEnabled(false);

}

//Speed down
void MainWindow::on_rewindPushButton_clicked()
{
    _paused = true;
    emit pauseTheSimulation();
    if( ui->forwardPushButton->isEnabled() == false )
        ui->forwardPushButton->setEnabled(true);

    _simulationSpeed += 200;
    _simulation->simulationSpeedChange(_simulationSpeed);
    on_playPushButton_clicked();

    //Minimum speed 2sec
    if(_simulationSpeed  == 2000)
        ui->rewindPushButton->setEnabled(false);
}

//Exit buttom
void MainWindow::on_actionExit_triggered()
{
    _exit = true;
    if(_simulation != nullptr && _paused == false)
    {
        //Emit a signal to simualtion stop, and waiting the finished() signal to exit
        emit pauseTheSimulation();
        return;
    }
    this->close();
}

//Exit functions------------------------------------------------
//If simulation stoped, exit application
void MainWindow::stopedSimAndExitApp()
{
    if(_exit) this->close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    event->isAccepted();
    on_actionExit_triggered();
}
//--------------------------------------------------------------

//Start Settings Dialog
void MainWindow::on_actionChange_parameters_triggered()
{
    bool push = false;
    if(_simulation != nullptr && _paused == false && _simulationFinished == false)
    {
        on_playPushButton_clicked();
        push = true;
    }

    bool cancel = true;
    _settingsDialog = new ParametersDialog(this,_xSize,_ySize,_fishNumber,_plantsNumber,_dryTheLake, cancel);
    _settingsDialog->exec();
    delete _settingsDialog;

    if(cancel)
    {
        //Continue simulation
        if(_simulation != nullptr && push == true)
            on_playPushButton_clicked();
    }
    else
    {
        resetVectorsAndData();
        inicializeTheLake();
    }

}
