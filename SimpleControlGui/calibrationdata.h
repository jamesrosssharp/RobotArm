#pragma once

#include <iostream>

struct CalibrationAngle
{
    float minDeg;
    float maxDeg;

    float minUs;
    float maxUs;

    void setDefaults()
    {
        minDeg = -90;
        maxDeg = 90;
        minUs = 1000;
        maxUs = 2000;
    }

};

struct Angle
{
    double deg;
    double us;
};

struct AngleGroup
{
    Angle base;
    Angle shoulder;
    Angle elbow;
    Angle wrist;
};

std::istream& operator >> (std::istream& is, CalibrationAngle& a);
std::ostream& operator << (std::ostream& is, CalibrationAngle& a);


class CalibrationData
{
public:

    void loadCalibrationData();

    void updateCalibrationData(CalibrationAngle base,
                               CalibrationAngle shoulder,
                               CalibrationAngle elbow,
                               CalibrationAngle wrist,
                               CalibrationAngle gripper);
    void getCalibrationData(CalibrationAngle& base,
                            CalibrationAngle& shoulder,
                            CalibrationAngle& elbow,
                            CalibrationAngle& wrist,
                            CalibrationAngle& gripper);

    void saveCalibrationData();

private:

    CalibrationAngle m_base;
    CalibrationAngle m_shoulder;
    CalibrationAngle m_elbow;
    CalibrationAngle m_wrist;
    CalibrationAngle m_gripper;

};

