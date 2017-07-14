#ifndef PARAMETERSDIALOG_H
#define PARAMETERSDIALOG_H

#include <QDialog>

namespace Ui {
class ParametersDialog;
}

class ParametersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParametersDialog(QWidget *parent = 0);
    ~ParametersDialog();

private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonOK_clicked();

private:
    Ui::ParametersDialog *ui;
};

#endif // PARAMETERSDIALOG_H
