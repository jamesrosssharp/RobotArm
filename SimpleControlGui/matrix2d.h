#pragma once

#include "vector2d.h"

class Matrix2D
{
public:
    Matrix2D(double a, double b, double c, double d);

    static Matrix2D createRot(double ang);

    friend  Matrix2D operator * (const Matrix2D& a, const Matrix2D& b);
    friend  Vector2D operator * (const Matrix2D& a, const Vector2D& b);

private:

    double m_a;
    double m_b;
    double m_c;
    double m_d;
};

Matrix2D operator * (const Matrix2D& a, const Matrix2D& b);
Vector2D operator * (const Matrix2D& a, const Vector2D& b);
