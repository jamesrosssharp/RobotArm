#include "matrix.h"
#include "matrix2d.h"

// m is row, n is column

Matrix::Matrix()
{

}

Matrix::Matrix(int m, int n, std::vector<std::vector<double>>& values)    :
    m_values(values),
    m_m(m),
    m_n(n)
{

}

Matrix Matrix::transpose()
{
    Matrix out;

    out.m_m = m_n;
    out.m_n = m_m;

    for (int i = 0; i < m_n; i++)
    {
        std::vector<double> row;
        for (int j = 0; j < m_m; j++)
        {
            row.push_back(getElem(j, i));
        }
        out.m_values.push_back(row);
    }
    return out;
}

Matrix Matrix::inverse()
{
    Matrix out;

    if (m_m == 2 && m_n == 2)
    {
        Matrix2D mat2d = toMatrix2D();
        Matrix2D inv = mat2d.inverse();
        out.fromMatrix2D(inv);
    }
    else
    {
        throw std::runtime_error("Don't know how to compute inverse!");
    }

    return out;
}

Matrix operator * (const Matrix& a, const Matrix& b)
{
    if (a.m_n != b.m_m) // number of columns must match number of rows
        throw std::runtime_error("Matrices not compatible!");


    Matrix out;

    out.m_m = a.m_m;
    out.m_n = b.m_n;

    for (int i = 0; i < a.m_m; i ++)
    {
        std::vector<double> row;
        for (int j = 0; j < b.m_n; j++)
        {
            double sum = 0.0;

            for (int k = 0; k < a.m_n; k++)
            {
                    sum += a.getElem(i, k) * b.getElem(k, j);
            }

            row.push_back(sum);
        }
        out.m_values.push_back(row);
    }

    return out;
}

double Matrix::getElem(int m, int n) const
{
    return m_values[m][n];
}

std::ostream& operator << (std::ostream& os, Matrix& a)
{
    for (int i = 0; i < a.m_m; i++)
    {
        os << "|";

        for (int j = 0; j < a.m_n; j ++)
        {
            os << a.getElem(i, j) << " ";
        }
        os << "|" << std::endl;
    }
    return os;
}

Matrix2D Matrix::toMatrix2D()
{
    Matrix2D out(getElem(0, 0), getElem(0, 1), getElem(1, 0), getElem(1, 1));

    return out;
}

void Matrix::fromMatrix2D(Matrix2D& in)
{
    m_values.clear();
    std::vector<double> row1 = {in.m_a, in.m_b};
    m_values.push_back(row1);
    std::vector<double> row2 = {in.m_c, in.m_d};
    m_values.push_back(row2);
    m_m = 2;
    m_n = 2;
}



