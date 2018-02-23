#pragma once

#include <iostream>

class Vector2D
{

public:

    Vector2D(double x, double y);

    friend Vector2D operator + (const Vector2D& a, const Vector2D& b);
    friend Vector2D operator * (const double a, const Vector2D& b);
    friend std::ostream& operator << (std::ostream& os, const Vector2D& v);

    double x() const { return m_x; }
    double y() const { return m_y; }

private:

    double m_x;
    double m_y;

};

Vector2D operator + (const Vector2D& a, const Vector2D& b);
Vector2D operator * (const double a, const Vector2D& b);

std::ostream& operator << (std::ostream& os, const Vector2D& v);
