#ifndef CALIBRATIONDONEDIALOG_H
#define CALIBRATIONDONEDIALOG_H

#include <QDialog>

namespace Ui {
class CalibrationDoneDialog;
}

class CalibrationDoneDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationDoneDialog(QWidget *parent = 0);
    ~CalibrationDoneDialog();

private:
    Ui::CalibrationDoneDialog *ui;
};

#endif // CALIBRATIONDONEDIALOG_H
