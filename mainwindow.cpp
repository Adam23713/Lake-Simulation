#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <typeinfo>

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

        //Read and add new fish species
        std::set<QString> names;
        while(!in.atEnd())
        {
            line = in.readLine();
            names.insert(line);
            //_fishiesName.insert(line.toStdWString()); //Convert QString to std::wstring
        }
        inputFile.close();

        //Delete old Object and datas
        if(_fileIsLoaded)
        {
            for(auto i : _picturelabels)
                delete i;

            //Resize the vectors
            _picturelabels.resize(0);
            _fishiesName.resize(0);
        }
        _fileIsLoaded = true;

        //Convert std::set to std::vector
        for(auto i : names)
            _fishiesName.push_back(i.toStdWString());

        //Drawn the lake and enabled the start button
        drawnTheWaterElement();
        ui->playPushButton->setEnabled(true);
        //********************************************
    }
    else
        QMessageBox::critical(this,"File error","I can't open the file!");
}

//Start/pause the simulation
void MainWindow::on_playPushButton_clicked()
{

    //drawnTheWaterElement();

    /*_simulation = new LakeSimulation(_xSize, _ySize);
    _simulation->init();
    _simulation->start();*/
}

void MainWindow::drawnTheWaterElement()
{
    //Drawn the clean lake
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

    //Drawn the Water object
    RandomWaterObjectFactory *factory = new RandomWaterObjectFactory(_fishiesName,_xSize,_ySize,6,1);
    _waterObjects = factory->makeWaterObjectVector();

    for(auto i : _waterObjects)
    {
        Point2D point = i->GetPosition();
        QString path;

        WaterObjectType var = i->GetType();
        switch(var)
        {
        case WaterObjectType::PLANT:
            path=":/images/images/plant.png";break;

        case WaterObjectType::FISH:
            SPECIES value = (static_cast<Fish*>(i))->GetSpecies();
            switch(value)
            {
            case SPECIES::Carnivorous:
                path = ":/images/images/Carnivorous.png"; break;
            case SPECIES::Herbivorous:
                path = ":/images/images/Herbivorous.png"; break;
            case  SPECIES::Omnivorous:
                path = ":/images/images/Omnivorous.png"; break;
            }break; //switch end
        }

        QPixmap image(path);
        QLabel* label = new QLabel(this);
        label->setPixmap(image);
        label->show();
        ui->lakeGrid->addWidget(label,point.GetXPosition(),point.GetYPosition());
        _waterObjectLabels.push_back(label);
    }
}
