#include "kinematics.h"

#include "vector2d.h"
#include "matrix2d.h"

#include <cmath>

const double armLengthA = 104.0;    // height of shoulder joint
const double armLengthB = 100.0;    // elbow and shoulder length
const double armLengthC = 98.0;     // distance from wrist joint to tip of gripper

const double pi = 3.14159265358;

static inline double TO_RAD(double ang)
{
    return ang * 2 * pi / 360.0;
}

static inline double TO_DEG(double ang)
{
    return ang * 360.0 / (2.0 * pi);
}

void Kinematics::computeForwardCyl(double base, double shoulder, double elbow, double wrist,
                              double& theta, double& rho, double& h)
{

    double s = TO_RAD(shoulder);
    double e = TO_RAD(elbow);
    double wr = TO_RAD(wrist);


    Vector2D y = Vector2D(0, armLengthA) + armLengthB * Vector2D(-std::sin(s), std::cos(s));

    Matrix2D r1 = Matrix2D::createRot(s);
    Vector2D z = y + r1 * (armLengthB * Vector2D(-std::sin(e), std::cos(e)));

    Matrix2D r2 = Matrix2D::createRot(e + s);
    Vector2D w = z + r2 * (armLengthC * Vector2D(-std::sin(wr), std::cos(wr)));

    rho = -w.x();
    theta = base;
    h = w.y();
}

void Kinematics::computeForwardCartesian(double base, double shoulder, double elbow,
                                         double wrist, double &x, double &y, double &z)
{
    double rho, theta, h;

    computeForwardCyl(base, shoulder, elbow, wrist, theta, rho, h);

    x = rho * std::sin(TO_RAD(theta));
    y = rho * std::cos(TO_RAD(theta));
    z = h;
}

bool Kinematics::computeInverseCyl(double rho, double theta, double h,
                              double& base, double& shoulder, double& elbow, double& wrist)
{

    if (rho < 50.0 || rho > 200.0 || h < 0.0 || h > 200.0)
    {
        return false;
    }

    double hprime = h + armLengthC - armLengthA;

    if (hprime > rho)
        return false;

    double w = std::sqrt(rho*rho + hprime*hprime);
    double gamma = std::acos(w / (2.0*armLengthB));
    double beta = std::acos(1.0 - w*w/(2.0*armLengthB*armLengthB));
    double zeta = std::atan(hprime / rho);
    double alpha = pi - gamma - beta;

    double theta2 = pi/2.0 - alpha - zeta;
    double phi = pi - beta;

    shoulder = TO_DEG(theta2);
    elbow = TO_DEG(phi);
    base = theta;
    wrist = 180 - shoulder - elbow;

    std::cout << " w: " << w << " gamma: " << gamma << " beta: " << beta << " alpha: " << alpha << " zeta:" << zeta << std::endl;

    return true;
}

bool Kinematics::computeInverseCartesian(double x, double y, double z, double &base,
                                         double &shoulder, double &elbow, double &wrist)
{

    double rho, theta, h;

    h = z;
    rho = std::sqrt(x*x + y*y);
    theta = TO_DEG(std::atan(x / y));

    return computeInverseCyl(rho, theta, h, base, shoulder, elbow, wrist);

}
