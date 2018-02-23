#pragma once

class HomePosition
{

public:

    void loadHomePosition();
    void saveHomePosition();

    void getHomeAngles(double& base, double& shoulder, double& elbow, double& wrist, double& gripper);
    void setHomeAngles(double base, double shoulder, double elbow, double wrist, double gripper);

private:

    double m_base;
    double m_shoulder;
    double m_elbow;
    double m_wrist;
    double m_gripper;

};
