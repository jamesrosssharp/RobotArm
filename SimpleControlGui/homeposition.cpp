#include "homeposition.h"

#include <fstream>
#include <iostream>

void HomePosition::loadHomePosition()
{

    std::ifstream datafile;

    try
    {
        datafile.open("home.txt");
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
    m_base      = 0;
    m_shoulder  = 0;
    m_elbow     = 0;
    m_wrist     = 0;
    m_gripper   = 0;
    return;
}

void HomePosition::saveHomePosition()
{
    std::ofstream datafile;

    datafile.open("home.txt");

    datafile << m_base << " ";
    datafile << m_shoulder << " ";
    datafile << m_elbow << " ";
    datafile << m_wrist << " ";
    datafile << m_gripper << std::endl;

}

void HomePosition::getHomeAngles(double& base, double& shoulder, double& elbow, double& wrist, double& gripper)
{
    base = m_base;
    shoulder = m_shoulder;
    elbow = m_elbow;
    wrist = m_wrist;
    gripper = m_gripper;
}

void HomePosition::setHomeAngles(double base, double shoulder, double elbow, double wrist, double gripper)
{
    m_base = base;
    m_shoulder = shoulder;
    m_elbow = elbow;
    m_wrist = wrist;
    m_gripper = gripper;
}
