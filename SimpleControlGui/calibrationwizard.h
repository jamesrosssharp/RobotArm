#pragma once

#include "robotcontroller.h"
#include "calibrationdata.h"

#include "calibrationadjustmentdialog.h"
#include "calibrationresultsdialog.h"

#include <QObject>
#include <QTimer>

class CalibrationWizard : public QObject
{
    Q_OBJECT

public:
    explicit CalibrationWizard(QWidget* parent, RobotController* robocon, CalibrationData* cal);
    ~CalibrationWizard();

    void doWizard();
public slots:

    void onDialogAccepted();
    void onDialogRejected();
    void update();

private:

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

    QTimer* m_timer;

    AngleGroup a1Angles;
    AngleGroup a8Angles;
    AngleGroup h8Angles;
    AngleGroup h1Angles;

    bool m_cancel = false;
};

