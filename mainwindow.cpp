#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <typeinfo>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    _fileIsLoaded = false;
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
}

MainWindow::~MainWindow()
{
    for(auto i : _picturelabels) delete i;
    qDebug() << "delete0";
    deleteGridMap();
    qDebug() << "delete1";
    for(auto i : _waterObjectLabels) delete i;
    qDebug() << "delete2";
    if(_simulation != nullptr)
    {
        disconnect(_simulation, SIGNAL(changeMap()), this, SLOT(updateLake()));
        disconnect(this,SIGNAL(pauseTheSimulation()), _simulation, SLOT(pauseTheSimulation()));
        delete _simulation;
    }
    qDebug() << "delete";
    delete ui;
}

//Open file, read it and drawn the lake
void MainWindow::on_actionOpen_File_triggered()
{
    if(_simulation != nullptr)
    {
        emit pauseTheSimulation();
        ui->playPushButton->setText("Play");
        ui->playPushButton->setIcon(QIcon(":/images/images/play.png"));
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
            //_fishiesName.insert(line.toStdWString()); //Convert QString to std::wstring
        }
        inputFile.close();

        //Delete old Object and datas------------------------------------------------
        if(_fileIsLoaded)
        {
            if(_simulation != nullptr)
            {
                disconnect(_simulation, SIGNAL(changeMap()), this, SLOT(updateLake()));
                disconnect(this,SIGNAL(pauseTheSimulation()), _simulation, SLOT(pauseTheSimulation()));
                disconnect(this,SIGNAL(drawnGidDone()),_simulation,SLOT(continueSimulation));
                delete _simulation;
                _simulation = nullptr;
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
        _fileIsLoaded = true;
        //-----------------------------------------------------------------------------

        //Convert std::set to std::vector
        for(auto i : names)
            _fishiesName.push_back(i.toStdWString());

        //Drawn the lake and enabled the start button
        drawnTheCleanWater();
        callFactory();
        createWaterObjectMap();
        drawnWaterElement();
        ui->playPushButton->setEnabled(true);
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
            ui->forwardPushButton->setEnabled(true);
            ui->rewindPushButton->setEnabled(true);
            _simulation->start();
        }
        return;
    }
    else
    {
        _simulation = new LakeSimulation(this, _xSize, _ySize, &_gridMap);
        connect(_simulation, SIGNAL(changeMap()), this, SLOT(updateLake()));
        connect(this,SIGNAL(pauseTheSimulation()), _simulation, SLOT(pauseTheSimulation()));

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

void MainWindow::drawnWaterElement()
{
    //Drawn Water elements
    for(unsigned int x = 0; x < _gridMap.size(); x++)
    {
        for(unsigned int y = 0; y < _gridMap.at(x).size(); y++)
        {
            if(_gridMap.at(x).at(y) == nullptr) continue;

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
    ui->lakeGrid->update();
}

void MainWindow::setSpritesAndToolTipStr(WaterObject* i, QString& path, QString& toolTip)
{
    WaterObjectType var = i->GetType();
    Point2D point = i->GetPosition();
    switch(var)
    {
        case WaterObjectType::PLANT:
            path=":/images/images/plant.png";
            toolTip = "Type: Plant\nName: Seaweed\nCoordinates: " + QString::number(point.GetXPosition()) + ";" + QString::number(point.GetYPosition());
            break;

        case WaterObjectType::FISH:
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
            }break; //switch end
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






