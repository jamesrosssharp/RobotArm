#pragma once

#include "robotcontroller.h"
#include "calibrationdata.h"

#include "calibrationadjustmentdialog.h"
#include "calibrationresultsdialog.h"
#include "calibrationdonedialog.h"
#include "chessboard.h"

#include <QObject>
#include <QTimer>
#include <utility>

class CalibrationWizard : public QObject
{
    Q_OBJECT

public:
    explicit CalibrationWizard(QWidget* parent, RobotController* robocon, CalibrationData* cal,  Chessboard* chessboard);
    ~CalibrationWizard();

    void doWizard();
public slots:

    void onDialogAccepted();
    void onDialogRejected();
    void update();

private:

    std::pair<double, double> calculateRegression(Angle& a1, Angle& a2, Angle& a3, Angle& a4);

    enum class State
    {
        kIdle,
        kStart,
        kStartWait,
        kCalibrate1,
        kCalibrate1Wait,
        kCalibrate2,
        kCalibrate2Wait,
        kCalibrate3,
        kCalibrate3Wait,
        kCalibrate4,
        kCalibrate4Wait,
        kDisplayResults1,
        kDisplayResults1Wait,
        kDisplayResults2,
        kDisplayResults2Wait,
        kDisplayResults3,
        kDisplayResults3Wait,
        kDisplayResults4,
        kDisplayResults4Wait,
        kDone,
        kDoneWait
    };

    State m_state;

    RobotController* m_robocon;
    CalibrationData* m_cal;

    CalibrationAdjustmentDialog* m_calAdjust;
    CalibrationResultsDialog* m_calResults;
    CalibrationDoneDialog* m_doneDialog;

    QTimer* m_timer;

    AngleGroup a1Angles;
    AngleGroup a8Angles;
    AngleGroup h8Angles;
    AngleGroup h1Angles;

    std::pair<double, double> m_baseReg;
    std::pair<double, double> m_shoulderReg;
    std::pair<double, double> m_wristReg;
    std::pair<double, double> m_elbowReg;


    bool m_cancel = false;
};

