#include "chessboard.h"

#include <iostream>
#include <fstream>

Chessboard::Chessboard()
{
    load();
}

void Chessboard::getSquareXYZ(int rank, int file, double &x, double &y, double &z)
{
    x = m_x[rank][file];
    y = m_y[rank][file];
    z = m_z[rank][file];
}

void Chessboard::getTrueSquareXYZ(int rank, int file, double &x, double &y, double &z)
{
    const double gridSpacing = 90.0 / 7.0;
    const double gridY = 85.0;

    x =  -41.0 + gridSpacing * file;
    y = (7 - rank) * gridSpacing + gridY;
    z = 15 + 12;

}

void Chessboard::setSquareCalibration(int rank, int file, double x, double y, double z)
{
    m_x[rank][file] = x;
    m_y[rank][file] = y;
    m_z[rank][file] = z;
}

void Chessboard::load()
{
    std::ifstream datafile;

    try
    {
        datafile.open("board.txt");
    }
    catch(...)
    {
        goto bail;
    }

    if (! datafile.is_open())
    {
        goto bail;
    }

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j ++)
        {
            datafile >> m_x[i][j];
            datafile >> m_y[i][j];
            datafile >> m_z[i][j];
        }

    return;

bail:
    setDefaults();

}

void Chessboard::save()
{
    std::ofstream datafile;

    datafile.open("board.txt");

    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j ++)
        {
            datafile << m_x[i][j] << " ";
            datafile << m_y[i][j] << " ";
            datafile << m_z[i][j] << std::endl;
        }

}

void Chessboard::setDefaults()
{


    for (int rank = 0; rank < 8; rank++)
        for (int file = 0; file < 8; file++)
        {

            getTrueSquareXYZ(rank, file, m_x[rank][file], m_y[rank][file], m_z[rank][file]);
        }
}

