#include "robotcontroller.h"
#include "kinematics.h"

#include <cmath>
#include <sstream>

const double kXOffset = 4.0;

RobotController::RobotController(CalibrationData* cal, HomePosition* home)   :
    m_cal(cal),
    m_home(home)
{

}

void RobotController::home()
{
    if (! initialHomingDone)
    {
        m_home->getHomeAngles(m_base, m_shoulder, m_elbow, m_wrist, m_gripper);
        initialHomingDone = true;
    }
    else
    {
        double base, shoulder, elbow, wrist, gripper;

        m_home->getHomeAngles(base, shoulder, elbow, wrist, gripper);

        // move gracefully to homing position
        moveTo(base, shoulder, elbow, wrist, gripper);
    }
}

void RobotController::getPositionCylindrical(double& rho, double& theta, double& h)
{
      // Forward kinematics: rho, theta and h from joint angles

    Kinematics::computeForwardCyl(m_base, m_shoulder, m_elbow, m_wrist, theta, rho, h);

}

void RobotController::getPositionCartesian(double& x, double& y, double& z)
{
    // Forward kinematics: x, y, and z from joint angles

    Kinematics::computeForwardCartesian(m_base, m_shoulder, m_elbow, m_wrist, x, y, z);
    x += kXOffset;
}

void RobotController::getPositionAngles(double& baseAng, double& baseUs,
                       double& shoulderAng, double& shoulderUs,
                       double& elbowAng, double& elbowUs,
                       double& wristAng, double& wristUs,
                       double& gripperAng, double& gripperUs)
{

    CalibrationAngle base, shoulder, elbow, wrist, gripper;

    m_cal->getCalibrationData(base, shoulder, elbow, wrist, gripper);

    std::vector<std::tuple<CalibrationAngle*, double*, double*, double*, bool>> vec =
    {std::make_tuple(&base, &m_base, &baseAng, &baseUs, false),
     std::make_tuple(&shoulder, &m_shoulder, &shoulderAng, &shoulderUs, false),
     std::make_tuple(&elbow, &m_elbow, &elbowAng, &elbowUs, false),
     std::make_tuple(&wrist, &m_wrist, &wristAng, &wristUs, true),
     std::make_tuple(&gripper, &m_gripper, &gripperAng, &gripperUs, false)};

    for (auto& v : vec)
    {
        CalibrationAngle* c = std::get<0>(v);
        double* a = std::get<1>(v);
        double* aout = std::get<2>(v);
        double* usout = std::get<3>(v);
        bool isWrist = std::get<4>(v);

        *aout = *a;
        *usout = lerpAngle(*c, *a + (isWrist ? m_wristAdjust : 0.0));
    }


}

void RobotController::setJointAngles(double base, double shoulder, double elbow,
                    double wrist, double gripper)
{

    moveTo(base, shoulder, elbow, wrist, gripper);
}

void RobotController::moveToCartesian(double x, double y, double z)
{

    if (m_state == State::kMoving)
    {
        m_cartesianMovementQueue.push_back(std::make_tuple(x, y, z));
    }
    else
    {
        double xcur, ycur, zcur;

        getPositionCartesian(xcur, ycur, zcur);

        double dx = x - xcur;
        double dy = y - ycur;
        double dz = z - zcur;

        std::vector<double> deltaVec = {dx, dy, dz};

        // compute greatest delta

        double maxDelta = 0.0;

        for (double d : deltaVec)
        {
            if (std::abs(d) > maxDelta)
                maxDelta = std::fabs(d);
        }

        // calculate steps required given speed


        double steps = maxDelta / m_speed; // degrees / (degrees / step) = steps

        int isteps = std::ceil(steps);

        if (isteps < 1)
            isteps = 1;

        // calculate gradients

        std::vector<double> gvec;

        for (double d : deltaVec)
            gvec.push_back(d / (double)isteps);

        m_steps = isteps;

        m_x = xcur;
        m_y = ycur;
        m_z = zcur;
        m_gx = gvec[0];
        m_gy = gvec[1];
        m_gz = gvec[2];

        m_curstep = 0;

        // change to moving mode

        m_state = State::kMoving;
        m_lerpMode = LerpMode::kCartesian;
    }
}

void RobotController::moveToCylindrical(double rho, double theta, double h)
{
    double rcur, tcur, hcur;

    getPositionCylindrical(rcur, tcur, hcur);

    double dr = rho - rcur;
    double dt = theta - tcur;
    double dh = h - hcur;

    std::vector<double> deltaVec = {dr, dt, dh};

    // compute greatest delta

    double maxDelta = 0.0;

    for (double d : deltaVec)
    {
        if (std::abs(d) > maxDelta)
            maxDelta = std::fabs(d);
    }

    // calculate steps required given speed


    double steps = maxDelta / m_speed; // degrees / (degrees / step) = steps

    int isteps = std::ceil(steps);

    if (isteps < 1)
        isteps = 1;

    // calculate gradients

    std::vector<double> gvec;

    for (double d : deltaVec)
        gvec.push_back(d / (double)isteps);

    m_steps = isteps;

    m_rho = rcur;
    m_theta = tcur;
    m_h = hcur;
    m_grho = gvec[0];
    m_gtheta = gvec[1];
    m_gh = gvec[2];

    m_curstep = 0;

    // change to moving mode

    m_state = State::kMoving;
    m_lerpMode = LerpMode::kCyl;

}

bool RobotController::computeInverseCartesian(double x, double y, double z,
                              double& base, double& shoulder, double& elbow, double& wrist)
{
    Kinematics::computeInverseCartesian(x - kXOffset, y, z, base, shoulder, elbow, wrist);
}


RobotController::ReturnCode RobotController::update()
{
    ReturnCode code;

    switch (m_state)
    {
        case State::kIdle:
        {
            code = ReturnCode::kIdle;
            break;
        }
        case State::kMoving:
        {
            if (m_curstep == m_steps)
            {
                serialWriteAngles();

                code = ReturnCode::kDone;
                m_state = State::kIdle;

                if (m_cartesianMovementQueue.size() != 0)
                {
                    std::tuple<double, double, double> t = m_cartesianMovementQueue.front();
                    m_cartesianMovementQueue.pop_front();
                    moveToCartesian(std::get<0>(t), std::get<1>(t), std::get<2>(t));
                }
            }
            else
            {
                switch (m_lerpMode)
                {
                    case LerpMode::kAngles:

                        m_base += m_gbase;
                        m_shoulder += m_gshoulder;
                        m_elbow += m_gelbow;
                        m_wrist += m_gwrist;
                        m_gripper += m_ggripper;
                        break;
                    case LerpMode::kCartesian:

                        m_x += m_gx;
                        m_y += m_gy;
                        m_z += m_gz;

                        computeInverseCartesian(m_x, m_y, m_z, m_base, m_shoulder, m_elbow, m_wrist);

                        break;
                    case LerpMode::kCyl:
                        m_rho += m_grho;
                        m_theta += m_gtheta;
                        m_h += m_gh;

                        Kinematics::computeInverseCyl(m_rho, m_theta, m_h, m_base, m_shoulder, m_elbow, m_wrist);

                        break;

                }

                // Output angles to serial
                serialWriteAngles();

                m_curstep++;

                code = ReturnCode::kMoving;


            }

            break;
        }
    }

    return code;
}

double RobotController::lerpAngle(CalibrationAngle& cal, double ang)
{
    /*if (ang < cal.minDeg)
        ang = cal.minDeg;

    if (ang > cal.maxDeg)
        ang = cal.maxDeg;
*/

    return cal.minUs + (cal.maxUs - cal.minUs)/(cal.maxDeg - cal.minDeg)*(ang - cal.minDeg);
}

bool RobotController::init()
{
    m_serialPort.setBaudRate(QSerialPort::Baud9600);
    m_serialPort.setPortName("/dev/ttyACM0");

    if (! m_serialPort.open(QIODevice::ReadWrite))
    {
        std::cout << "Could not open serial port!" << std::endl;
        return false;
    }

    m_serialPortOpen = true;

    return true;
}

void RobotController::moveTo(double base, double shoulder, double elbow, double wrist, double gripper)
{
    // compute deltas

    double db = base - m_base;
    double ds = shoulder - m_shoulder;
    double de = elbow - m_elbow;
    double dw = wrist - m_wrist;
    double dg = gripper - m_gripper;

    std::vector<double> deltaVec = {db, ds, de, dw, dg};

    // compute greatest delta

    double maxDelta = 0.0;

    for (double d : deltaVec)
    {
        if (std::abs(d) > maxDelta)
            maxDelta = std::fabs(d);
    }

    // calculate steps required given speed


    double steps = maxDelta / m_speed; // degrees / (degrees / step) = steps

    int isteps = std::ceil(steps);

    if (isteps < 1)
        isteps = 1;

    // calculate gradients

    std::vector<double> gvec;

    for (double d : deltaVec)
        gvec.push_back(d / (double)isteps);

    m_steps = isteps;
    m_gbase     = gvec[0];
    m_gshoulder = gvec[1];
    m_gelbow    = gvec[2];
    m_gwrist    = gvec[3];
    m_ggripper     = gvec[4];
    m_curstep = 0;

    // change to moving mode

    m_state = State::kMoving;
    m_lerpMode = LerpMode::kAngles;

}

void RobotController::serialWriteAngles()
{

    double baseAng, baseUs;
    double shoulderAng, shoulderUs;
    double elbowAng, elbowUs;
    double wristAng, wristUs;
    double gripperAng, gripperUs;


    getPositionAngles(baseAng, baseUs,
                      shoulderAng, shoulderUs,
                      elbowAng, elbowUs,
                      wristAng, wristUs,
                      gripperAng, gripperUs);

    std::stringstream ss;

    ss << int(std::round(baseUs)) << " " << int(std::round(shoulderUs)) << " " <<
          int(std::round(elbowUs)) << " " << int(std::round(wristUs)) << " " <<
          int(std::round(gripperUs)) << std::endl;

    if (m_serialPortOpen)
    {
        std::cout << "Writing to serial: " << ss.str();

        m_serialPort.write(ss.str().c_str(), ss.str().size());

        char data[1024];
        m_serialPort.readLine(data, 1023);
        data[1023] = '\0';
        std::cout << data << std::endl;
        fflush(0);
    }
    else
    {
        std::cout << ss.str();
        fflush(0);
    }
}
