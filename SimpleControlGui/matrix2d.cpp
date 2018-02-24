#include "matrix2d.h"

#include <cmath>

Matrix2D::Matrix2D(double a, double b, double c, double d)  :
    m_a(a),
    m_b(b),
    m_c(c),
    m_d(d)
{

}

Matrix2D Matrix2D::createRot(double ang)
{
    return Matrix2D(std::cos(ang), -std::sin(ang), std::sin(ang), std::cos(ang));
}

Matrix2D operator * (const Matrix2D& a, const Matrix2D& b)
{
    return Matrix2D(a.m_a*b.m_a + a.m_b*b.m_c,
                    a.m_a*b.m_b + a.m_b*b.m_d,
                    a.m_c*b.m_a + a.m_d*b.m_c,
                    a.m_c*b.m_b + a.m_d*b.m_d);
}

Vector2D operator * (const Matrix2D& a, const Vector2D& b)
{
    return Vector2D(a.m_a*b.x() + a.m_b*b.y(), a.m_c*b.x() + a.m_d*b.y());
}

Matrix2D Matrix2D::inverse()
{
    double det = m_a * m_d - m_b * m_c;

    if (det == 0.0)
        throw std::runtime_error("Matrix was singular");

    Matrix2D out(m_d / det, -m_b / det, -m_c / det, m_a / det);

    return out;
}
