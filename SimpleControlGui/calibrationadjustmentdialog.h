#ifndef CALIBRATIONADJUSTMENTDIALOG_H
#define CALIBRATIONADJUSTMENTDIALOG_H

#include <QDialog>

#include "calibrationdata.h"
#include "robotcontroller.h"
#include "chessboard.h"

namespace Ui {
class CalibrationAdjustmentDialog;
}

class CalibrationAdjustmentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationAdjustmentDialog(RobotController* robocon, Chessboard* chessboard, QWidget *parent = 0);
    ~CalibrationAdjustmentDialog();

    void setSquare(int rank, int file);
    void doDialog(bool isTrue = true);

    void getCalibration(Angle& base, Angle& shoulder, Angle& elbow, Angle& wrist);
public slots:

    void onXPlus();
    void onXMinus();

    void onYPlus();
    void onYMinus();

    void onZPlus();
    void onZMinus();

    void onDone();

    void onWristPlus();
    void onWristMinus();

private:
    Ui::CalibrationAdjustmentDialog *ui;

    int m_rank;
    int m_file;

    double m_x;
    double m_y;
    double m_z;

    Angle m_base;
    Angle m_shoulder;
    Angle m_elbow;
    Angle m_wrist;

    RobotController* m_robocon;

    bool m_moveBeforeAdjust;

    Chessboard* m_chessboard;

};

#endif // CALIBRATIONADJUSTMENTDIALOG_H
