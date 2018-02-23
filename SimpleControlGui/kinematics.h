#pragma once

class Kinematics
{
public:
    static void computeForwardCyl(double base, double shoulder, double elbow, double wrist,
                                  double& theta, double& rho, double& h);
    static void computeForwardCartesian(double base, double shoulder, double elbow, double wrist,
                                  double& x, double& y, double& z);

    static bool computeInverseCyl(double rho, double theta, double h,
                                  double& base, double& shoulder, double& elbow, double& wrist);
    static bool computeInverseCartesian(double x, double y, double z,
                                  double& base, double& shoulder, double& elbow, double& wrist);

};
