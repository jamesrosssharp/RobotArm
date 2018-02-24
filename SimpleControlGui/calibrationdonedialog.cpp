#include "calibrationdonedialog.h"
#include "ui_calibrationdonedialog.h"

CalibrationDoneDialog::CalibrationDoneDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDoneDialog)
{
    ui->setupUi(this);
}

CalibrationDoneDialog::~CalibrationDoneDialog()
{
    delete ui;
}
