#include "calibrationwizard.h"
#include "leastsquares.h"

CalibrationWizard::CalibrationWizard(QWidget* parent, RobotController* robocon, CalibrationData* cal,  Chessboard* chessboard)    :
    m_robocon(robocon),
    m_cal(cal),
    m_calAdjust(new CalibrationAdjustmentDialog(robocon, chessboard, parent)),
    m_calResults(new CalibrationResultsDialog(parent)),
    m_doneDialog(new CalibrationDoneDialog(parent)),
    m_timer(new QTimer)
{

    connect(m_calAdjust, SIGNAL(accepted()), this, SLOT(onDialogAccepted()));
    connect(m_calAdjust, SIGNAL(rejected()), this, SLOT(onDialogRejected()));

    connect(m_calResults, SIGNAL(accepted()), this, SLOT(onDialogAccepted()));
    connect(m_calResults, SIGNAL(rejected()), this, SLOT(onDialogRejected()));

    connect(m_doneDialog, SIGNAL(rejected()), this, SLOT(onDialogRejected()));
    connect(m_doneDialog, SIGNAL(accepted()), this, SLOT(onDialogAccepted()));


    // We need to set a timer to run the wizard asynchronously on the UI thread.
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));

    m_timer->start(100);
}

CalibrationWizard::~CalibrationWizard()
{
    // don't delete calAdjust or calResults: they will be deleted by Qt when parent is deleted.

    m_timer->stop();
    delete m_timer;
}

void CalibrationWizard::onDialogAccepted()
{
    printf("Dialog accepted"); fflush(0);

    switch(m_state)
    {
        case State::kCalibrate1Wait:
            m_calAdjust->getCalibration(a1Angles.base, a1Angles.shoulder, a1Angles.elbow, a1Angles.wrist);
            m_state = State::kCalibrate2;
            break;
        case State::kCalibrate2Wait:
            m_calAdjust->getCalibration(h1Angles.base, h1Angles.shoulder, h1Angles.elbow, h1Angles.wrist);
            m_state = State::kCalibrate3;
            break;
        case State::kCalibrate3Wait:
            m_calAdjust->getCalibration(h8Angles.base, h8Angles.shoulder, h8Angles.elbow, h8Angles.wrist);
            m_state = State::kCalibrate4;
            break;
        case State::kCalibrate4Wait:
            m_calAdjust->getCalibration(a8Angles.base, a8Angles.shoulder, a8Angles.elbow, a8Angles.wrist);
            m_state = State::kDisplayResults1;
            break;
        case State::kDisplayResults1Wait:
            m_state = State::kDisplayResults2;
            break;
        case State::kDisplayResults2Wait:
            m_state = State::kDisplayResults3;
            break;
        case State::kDisplayResults3Wait:
            m_state = State::kDisplayResults4;
            break;
        case State::kDisplayResults4Wait:
            m_state = State::kDone;
            break;
        case State::kDoneWait:
            // Save calibration

            m_cal->updateFromRegressions(m_baseReg, m_shoulderReg, m_elbowReg, m_wristReg);
            m_cal->saveCalibrationData();

            m_state = State::kIdle;
            break;
        default:
            break;
    }

}

void CalibrationWizard::onDialogRejected()
{
    printf("Dialog rejected"); fflush(0);
    m_cancel = true;
    m_state = State::kIdle;
}

void CalibrationWizard::doWizard()
{
    m_state = State::kStart;
    m_robocon->setWristAdjust(0.0);
}

void CalibrationWizard::update()
{
    switch (m_state)
    {
        case State::kStart:
            m_state = State::kCalibrate1;
            break;
        case State::kCalibrate1:
            m_calAdjust->setSquare(0,0);
            m_calAdjust->doDialog();
            m_state = State::kCalibrate1Wait;
            break;
        case State::kCalibrate2:
            m_calAdjust->setSquare(7,0);
            m_calAdjust->doDialog();
            m_state = State::kCalibrate2Wait;
            break;
        case State::kCalibrate3:
            m_calAdjust->setSquare(7,7);
            m_calAdjust->doDialog();
            m_state = State::kCalibrate3Wait;
            break;
        case State::kCalibrate4:
            m_calAdjust->setSquare(0,7);
            m_calAdjust->doDialog();
            m_state = State::kCalibrate4Wait;
            break;
        case State::kDisplayResults1:
            m_baseReg = calculateRegression(a1Angles.base, h1Angles.base, h8Angles.base, a8Angles.base);

            m_calResults->setJoint("base", a1Angles.base, h1Angles.base, h8Angles.base, a8Angles.base, m_baseReg.first, m_baseReg.second);
            m_calResults->show();
            m_state = State::kDisplayResults1Wait;
            break;
        case State::kDisplayResults2:

            m_shoulderReg = calculateRegression(a1Angles.shoulder, h1Angles.shoulder, h8Angles.shoulder, a8Angles.shoulder);

            m_calResults->setJoint("shoulder", a1Angles.shoulder, h1Angles.shoulder, h8Angles.shoulder, a8Angles.shoulder, m_shoulderReg.first, m_shoulderReg.second);
            m_calResults->show();
            m_state = State::kDisplayResults2Wait;
            break;
        case State::kDisplayResults3:
            m_elbowReg = calculateRegression(a1Angles.elbow, h1Angles.elbow, h8Angles.elbow, a8Angles.elbow);

            m_calResults->setJoint("elbow", a1Angles.elbow, h1Angles.elbow, h8Angles.elbow, a8Angles.elbow, m_elbowReg.first, m_elbowReg.second);
            m_calResults->show();
            m_state = State::kDisplayResults3Wait;
            break;
        case State::kDisplayResults4:
            m_wristReg = calculateRegression(a1Angles.wrist, h1Angles.wrist, h8Angles.wrist, a8Angles.wrist);

            m_calResults->setJoint("wrist", a1Angles.wrist, h1Angles.wrist, h8Angles.wrist, a8Angles.wrist, m_wristReg.first, m_wristReg.second);
            m_calResults->show();
            m_state = State::kDisplayResults4Wait;
            break;
        case State::kDone:
            m_robocon->setWristAdjust(0.0);
            m_doneDialog->show();
            m_state = State::kDoneWait;
            break;
    }
}

std::pair<double, double> CalibrationWizard::calculateRegression(Angle& a1, Angle& a2, Angle& a3, Angle& a4)
{

    std::vector<double> x = {a1.deg, a2.deg, a3.deg, a4.deg};
    std::vector<double> y = {a1.us, a2.us, a3.us, a4.us};

    double m, c;

    LeastSquares::compute(x, y, m, c);

    return {m, c};

}
