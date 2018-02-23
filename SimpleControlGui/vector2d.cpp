#include "vector2d.h"

Vector2D::Vector2D(double x, double y)  :
    m_x(x),
    m_y(y)
{

}


Vector2D operator + (const Vector2D& a, const Vector2D& b)
{
    return Vector2D(a.m_x + b.m_x, a.m_y + b.m_y);
}

Vector2D operator * (const double a, const Vector2D& b)
{
    return Vector2D(a * b.m_x, a * b.m_y);
}

std::ostream& operator << (std::ostream& os, const Vector2D& v)
{
    os << "(" << v.x() << "," << v.y() << ")";
    return os;
}
