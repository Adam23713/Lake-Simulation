#include "parametersdialog.h"
#include "ui_parametersdialog.h"

ParametersDialog::ParametersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ParametersDialog)
{
    ui->setupUi(this);
    ui->lineEditFish->setValidator( new QIntValidator(0, 500, this) );
    ui->lineEditPlant->setValidator( new QIntValidator(0, 500, this) );
    ui->lineEditX->setValidator( new QIntValidator(0, 50, this) );
    ui->lineEditY->setValidator( new QIntValidator(0, 50, this) );
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

}
