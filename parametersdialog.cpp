#include "parametersdialog.h"
#include "ui_parametersdialog.h"

ParametersDialog::ParametersDialog(QWidget *parent, int &x, int &y, int &fishN, int &plantsN, bool &dry, bool &cancel)
    : QDialog(parent), ui(new Ui::ParametersDialog), _xSize(x), _ySize(y), _fishNumber(fishN), _plantsNumber(plantsN), _dry(dry), _cancel(cancel)
{
    ui->setupUi(this);
    ui->lineEditFish->setValidator( new QIntValidator(0, 500, this) );
    ui->lineEditPlant->setValidator( new QIntValidator(0, 500, this) );
    ui->lineEditX->setValidator( new QIntValidator(0, 50, this) );
    ui->lineEditY->setValidator( new QIntValidator(0, 50, this) );

    ui->lineEditFish->setText(QString::number(_fishNumber));
    ui->lineEditPlant->setText(QString::number(_plantsNumber));
    ui->lineEditX->setText(QString::number(_xSize));
    ui->lineEditY->setText(QString::number(_ySize));

    if(_dry)
        ui->checkBoxYes->setChecked(true);
    else
        ui->checkBoxNo->setChecked(true);
}

ParametersDialog::~ParametersDialog()
{
    delete ui;
}

void ParametersDialog::on_pushButtonCancel_clicked()
{
    this->close();
}

void ParametersDialog::on_pushButtonOK_clicked()
{
    _xSize = ui->lineEditX->text().toInt();
    _ySize = ui->lineEditY->text().toInt();
    _fishNumber = ui->lineEditFish->text().toInt();
    _plantsNumber = ui->lineEditPlant->text().toInt();

    if(ui->checkBoxYes->isChecked())
        _dry = true;
    else
        _dry = false;

    _cancel = false;
    this->close();
}

void ParametersDialog::on_checkBoxYes_clicked(bool checked)
{
    ui->checkBoxNo->setChecked(!checked);
}


void ParametersDialog::on_checkBoxNo_clicked(bool checked)
{
    ui->checkBoxYes->setChecked(!checked);
}
