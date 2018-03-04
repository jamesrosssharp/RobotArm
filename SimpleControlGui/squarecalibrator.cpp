#include "squarecalibrator.h"

#include <iostream>

SquareCalibrator::SquareCalibrator(Chessboard* board, RobotController* robocon, QWidget* parent)    :
        m_cal(new CalibrationAdjustmentDialog(robocon, board, parent)),
        m_chessboard(board),
        m_robocon(robocon)
{
    connect(m_cal, SIGNAL(accepted()), this, SLOT(onAccept()));
}

void SquareCalibrator::doDialog(int rank, int file)
{
    m_rank = rank;
    m_file = file;

    m_cal->setSquare(rank, file);
    m_cal->doDialog(false);
}

void SquareCalibrator::onAccept()
{
    double x, y, z;

    m_robocon->getPositionCartesian(x, y, z);

    std::cout << "New square cal: " << m_rank << " " << m_file << " " << x << " " << y << " " << z << std::endl;

    m_chessboard->setSquareCalibration(m_rank, m_file, x, y, z);
    m_chessboard->save();
}
