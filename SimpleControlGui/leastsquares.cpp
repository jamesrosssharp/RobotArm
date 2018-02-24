#include "leastsquares.h"

#include "matrix.h"

#include <iostream>

void LeastSquares::compute(std::vector<double>& x, std::vector<double>& y, double& m, double& c)
{

    if (x.size() != y.size())
        throw std::runtime_error("Sample number mismatch");


    std::vector<std::vector<double>> rows;

    for (int i = 0; i < x.size(); i ++)
    {
        std::vector<double> row = {1, x[i]};
        rows.push_back(row);
    }

    Matrix X(x.size(), 2, rows);

    std::cout << X << std::endl;

    Matrix X_t = X.transpose();

    std::cout << X_t << std::endl;

    Matrix X_t_X = X_t * X;

    std::cout << X_t_X << std::endl;

    Matrix X_t_X_inv = X_t_X.inverse();

    std::cout << X_t_X_inv << std::endl;

    std::vector<std::vector<double>> y_vec;

    for (int i = 0; i < y.size(); i++)
    {
        std::vector<double> row = {y[i]};
        y_vec.push_back(row);
    }

    Matrix Y(y.size(), 1, y_vec);

    Matrix betaHat = (X_t_X_inv * X_t) * Y;

    std::cout << betaHat << std::endl;

    m = betaHat.getElem(1, 0);
    c = betaHat.getElem(0, 0);

}

