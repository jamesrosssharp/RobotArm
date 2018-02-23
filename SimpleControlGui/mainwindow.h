#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "calibrationdialog.h"
#include "calibrationwizard.h"
#include "robotcontroller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool init();

public slots:
    void showCalibrationWindow();
    void onCalibrationDialogClosed();
    void update();
    void home();
    void onAngleChanged();
    void onSpeedChanged();
    void onCartGo();
    void onCylGo();
    void onGoSquare();
    void onCalibrationWizard();

private:

    void updatePrimaryWidgets();
    void updateSecondaryWidgets();
    void disablePrimaryWidgets();
    void enablePrimaryWidgets();

    Ui::MainWindow* ui;
    CalibrationDialog* cal;
    RobotController* robocon;
    CalibrationWizard* calwiz;
    QTimer* timer;
};

#endif // MAINWINDOW_H
