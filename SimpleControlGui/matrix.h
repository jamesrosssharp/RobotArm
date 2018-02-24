#pragma once

#include <vector>
#include <iostream>

#include "matrix2d.h"

class Matrix
{
public:
    Matrix();
    Matrix(int m, int n, std::vector<std::vector<double>>& values);

    Matrix transpose();
    Matrix inverse();

    Matrix2D toMatrix2D();
    void fromMatrix2D(Matrix2D& in);

    friend  Matrix operator * (const Matrix& a, const Matrix& b);
    friend  std::ostream& operator << (std::ostream& os, Matrix& a);

    double getElem(int m, int n) const;

private:


    std::vector<std::vector<double>> m_values;

    int m_m;
    int m_n;
};

Matrix operator * (const Matrix& a, const Matrix& b);
std::ostream& operator << (std::ostream& os, Matrix& a);
