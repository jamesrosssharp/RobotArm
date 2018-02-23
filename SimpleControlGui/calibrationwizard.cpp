#include "calibrationwizard.h"

CalibrationWizard::CalibrationWizard(QWidget* parent, RobotController* robocon, CalibrationData* cal)    :
    m_robocon(robocon),
    m_cal(cal),
    m_calAdjust(new CalibrationAdjustmentDialog(robocon, parent)),
    m_calResults(new CalibrationResultsDialog(parent)),
    m_timer(new QTimer)
{

    connect(m_calAdjust, SIGNAL(accepted()), this, SLOT(onDialogAccepted()));
    connect(m_calAdjust, SIGNAL(rejected()), this, SLOT(onDialogRejected()));

    connect(m_calResults, SIGNAL(accepted()), this, SLOT(onDialogAccepted()));
    connect(m_calResults, SIGNAL(rejected()), this, SLOT(onDialogRejected()));


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
            m_calAdjust->getCalibration(a1Angles.base, a1Angles.elbow, a1Angles.shoulder, a1Angles.wrist);
            m_state = State::kCalibrate2;
            break;
        case State::kCalibrate2Wait:
            m_calAdjust->getCalibration(h1Angles.base, h1Angles.elbow, h1Angles.shoulder, h1Angles.wrist);
            m_state = State::kCalibrate3;
            break;
        case State::kCalibrate3Wait:
            m_calAdjust->getCalibration(h8Angles.base, h8Angles.elbow, h8Angles.shoulder, h8Angles.wrist);
            m_state = State::kCalibrate4;
            break;
        case State::kCalibrate4Wait:
            m_calAdjust->getCalibration(a8Angles.base, a8Angles.elbow, a8Angles.shoulder, a8Angles.wrist);
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
            m_calResults->setJoint("base", a1Angles.base, h1Angles.base, h8Angles.base, a8Angles.base, 0, 0);
            m_calResults->show();
            m_state = State::kDisplayResults1Wait;
            break;
        case State::kDisplayResults2:
            m_calResults->setJoint("shoulder", a1Angles.shoulder, h1Angles.shoulder, h8Angles.shoulder, a8Angles.shoulder, 0, 0);
            m_calResults->show();
            m_state = State::kDisplayResults2Wait;
            break;
        case State::kDisplayResults3:
            m_calResults->setJoint("elbow", a1Angles.elbow, h1Angles.elbow, h8Angles.elbow, a8Angles.elbow, 0, 0);
            m_calResults->show();
            m_state = State::kDisplayResults3Wait;
            break;
        case State::kDisplayResults4:
            m_calResults->setJoint("wrist", a1Angles.wrist, h1Angles.wrist, h8Angles.wrist, a8Angles.wrist, 0, 0);
            m_calResults->show();
            m_state = State::kDisplayResults4Wait;
            break;
        case State::kDone:

            m_state = State::kDoneWait;
            break;
    }
}

