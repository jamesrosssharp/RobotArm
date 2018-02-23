#include "calibrationdata.h"

#include <iostream>
#include <fstream>

void CalibrationData::loadCalibrationData()
{

    std::ifstream datafile;

    try
    {
        datafile.open("cal.txt");
    }
    catch(...)
    {
        goto bail;
    }

    if (! datafile.is_open())
    {
        goto bail;
    }

    datafile >> m_base;
    datafile >> m_shoulder;
    datafile >> m_elbow;
    datafile >> m_wrist;
    datafile >> m_gripper;

    return;

bail:
    printf("Could not open file!\n"); fflush(0);
    m_base.setDefaults();
    m_shoulder.setDefaults();
    m_elbow.setDefaults();
    m_wrist.setDefaults();
    m_gripper.setDefaults();
    return;
}

void CalibrationData::updateCalibrationData(CalibrationAngle base,
                           CalibrationAngle shoulder,
                           CalibrationAngle elbow,
                           CalibrationAngle wrist,
                           CalibrationAngle gripper)
{
    m_base = base;
    m_shoulder = shoulder;
    m_elbow = elbow;
    m_wrist = wrist;
    m_gripper = gripper;
}

void CalibrationData::getCalibrationData(CalibrationAngle& base,
                        CalibrationAngle& shoulder,
                        CalibrationAngle& elbow,
                        CalibrationAngle& wrist,
                        CalibrationAngle& gripper)
{
    base = m_base;
    shoulder = m_shoulder;
    elbow = m_elbow;
    wrist = m_wrist;
    gripper = m_gripper;
}

void CalibrationData::saveCalibrationData()
{
    std::ofstream datafile;

    datafile.open("cal.txt");

    datafile << m_base;
    datafile << m_shoulder;
    datafile << m_elbow;
    datafile << m_wrist;
    datafile << m_gripper;

}

std::istream& operator >> (std::istream& is, CalibrationAngle& a)
{
    is >> a.minDeg;
    is >> a.maxDeg;
    is >> a.minUs;
    is >> a.maxUs;
    return is;
}

std::ostream& operator << (std::ostream& os, CalibrationAngle& a)
{
    os << a.minDeg << " ";
    os << a.maxDeg << " ";
    os << a.minUs << " ";
    os << a.maxUs << std::endl;
    return os;
}



