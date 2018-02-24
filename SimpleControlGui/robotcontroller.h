#pragma once

#include "calibrationdata.h"
#include "homeposition.h"

#include <QSerialPort>

#include <deque>
#include <tuple>

class RobotController
{
public:

    RobotController(CalibrationData* cal, HomePosition* home);

    void home();
    bool init();

    void getPositionCylindrical(double& rho, double& theta, double& h);
    void getPositionCartesian(double& x, double& y, double& z);
    void getPositionAngles(double& baseAng, double& baseUs,
                           double& shoulderAng, double& shoulderUs,
                           double& elbowAng, double& elbowUs,
                           double& wristAng, double& wristUs,
                           double& gripperAng, double& gripperUs);

    void setJointAngles(double base, double shoulder, double elbow,
                        double wrist, double gripper);
    void moveToCartesian(double x, double y, double z);
    void moveToCylindrical(double rho, double theta, double h);

    void setWristAdjust(double w) { m_wristAdjust = w; moveTo(m_base, m_shoulder, m_elbow, m_wrist, m_gripper); }
    double getWristAdjust() { return m_wristAdjust; }

    void setSpeed(double speed) { m_speed = speed; }

    bool computeInverseCartesian(double x, double y, double z,
                                  double& base, double& shoulder, double& elbow, double& wrist);


    enum class ReturnCode
    {
        kIdle,
        kMoving,
        kDone
    };

    enum class State
    {
        kIdle,
        kMoving
    };

    // return value true indicates editable widgets in UI need to be updated
    ReturnCode update();

private:

    double lerpAngle(CalibrationAngle& cal, double ang);
    void moveTo(double base, double shoulder, double elbow, double wrist, double gripper);
    void serialWriteAngles();

    CalibrationData* m_cal;
    HomePosition*    m_home;

    enum class LerpMode
    {
        kAngles,
        kCyl,
        kCartesian
    };

    LerpMode m_lerpMode;

    double m_base;
    double m_shoulder;
    double m_elbow;
    double m_wrist;
    double m_gripper;

    double m_gbase;
    double m_gshoulder;
    double m_gelbow;
    double m_gwrist;
    double m_ggripper;

    double m_x;
    double m_y;
    double m_z;

    double m_gx;
    double m_gy;
    double m_gz;

    double m_rho;
    double m_theta;
    double m_h;

    double m_grho;
    double m_gtheta;
    double m_gh;

    int m_steps;
    int m_curstep;

    double m_wristAdjust = 0.0;

    State m_state;

    QSerialPort m_serialPort;
    bool m_serialPortOpen = false;

    bool initialHomingDone = false;

    double m_speed = 1.0;

    std::deque<std::tuple<double, double, double>> m_cartesianMovementQueue;

};
