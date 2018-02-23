#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

#include <QDialog>

#include "calibrationdata.h"
#include "homeposition.h"

namespace Ui {
class CalibrationDialog;
}

class CalibrationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationDialog(QWidget *parent = 0);
    ~CalibrationDialog();

    CalibrationData* getCalData() { return data; }
    HomePosition* getHomePosition() { return home; }

public slots:
    void accept();
    void reject();

private:

    void loadData();

    Ui::CalibrationDialog *ui;
    CalibrationData* data;
    HomePosition* home;
};

#endif // CALIBRATIONDIALOG_H
