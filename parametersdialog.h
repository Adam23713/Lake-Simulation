#ifndef PARAMETERSDIALOG_H
#define PARAMETERSDIALOG_H

#include <QDialog>

//A settings menüpontért felelős osztály

namespace Ui {
class ParametersDialog;
}

class ParametersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParametersDialog(QWidget *parent, int& x, int& y, int& fishN, int& plantsN, bool& dry, bool& cancel);
    ~ParametersDialog();

private slots:
    void on_pushButtonCancel_clicked();
    void on_pushButtonOK_clicked();
    void on_checkBoxYes_clicked(bool checked);

    void on_checkBoxNo_clicked(bool checked);

private:
    Ui::ParametersDialog *ui;
    int& _xSize;
    int& _ySize;
    int& _fishNumber;
    int& _plantsNumber;
    bool& _dry;
    bool& _cancel;
};

#endif // PARAMETERSDIALOG_H
