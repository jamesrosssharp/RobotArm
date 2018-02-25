#pragma once

#include "chessboard.h"
#include "calibrationadjustmentdialog.h"
#include "robotcontroller.h"

#include <QObject>

class SquareCalibrator : public QObject
{
    Q_OBJECT
public:
    SquareCalibrator(Chessboard* board, RobotController* robocon, QWidget* parent);

    void doDialog(int rank, int file);

public slots:

    void onAccept();

private:
    CalibrationAdjustmentDialog* m_cal;
    Chessboard* m_chessboard;
    RobotController* m_robocon;

    int m_rank;
    int m_file;
};

