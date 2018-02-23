#include "calibrationadjustmentdialog.h"
#include "ui_calibrationadjustmentdialog.h"

#include "chessboard.h"
#include "kinematics.h"

const double kXInc = 0.5;
const double kYInc = 0.5;
const double kZInc = 0.5;

CalibrationAdjustmentDialog::CalibrationAdjustmentDialog(RobotController* robocon, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationAdjustmentDialog),
    m_robocon(robocon)
{
    ui->setupUi(this);
}

CalibrationAdjustmentDialog::~CalibrationAdjustmentDialog()
{
    delete ui;
}

void CalibrationAdjustmentDialog::setSquare(int rank, int file)
{
    m_rank = rank;
    m_file = file;

    ui->square->setText(QString("%1%2").arg((char)('a' + file)).arg(rank + 1));
}

void CalibrationAdjustmentDialog::doDialog()
{
    double x, y, z;

    Chessboard::getSquareXYZ(m_rank, m_file, x, y, z);

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));

    Kinematics::computeInverseCartesian(x, y, z, m_base.deg, m_shoulder.deg, m_elbow.deg, m_wrist.deg);

    show();
}

void CalibrationAdjustmentDialog::onXMinus()
{
    double x, y, z;

    m_robocon->moveToCartesian(0, 100, 60);
    m_robocon->getPositionCartesian(x, y, z);

    x -= kXInc;

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));

}

void CalibrationAdjustmentDialog::onXPlus()
{
    double x, y, z;

    m_robocon->moveToCartesian(0, 100, 60);
    m_robocon->getPositionCartesian(x, y, z);

    x += kXInc;

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));
}

void CalibrationAdjustmentDialog::onYMinus()
{
    double x, y, z;

    m_robocon->moveToCartesian(0, 100, 60);
    m_robocon->getPositionCartesian(x, y, z);

    y -= kYInc;

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));
}

void CalibrationAdjustmentDialog::onYPlus()
{
    double x, y, z;

    m_robocon->moveToCartesian(0, 100, 60);
    m_robocon->getPositionCartesian(x, y, z);

    y += kXInc;

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));
}

void CalibrationAdjustmentDialog::onZMinus()
{
    double x, y, z;

    m_robocon->moveToCartesian(0, 100, 60);
    m_robocon->getPositionCartesian(x, y, z);

    z -= kZInc;

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));
}

void CalibrationAdjustmentDialog::onZPlus()
{
    double x, y, z;

    m_robocon->moveToCartesian(0, 100, 60);
    m_robocon->getPositionCartesian(x, y, z);

    z += kZInc;

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));
}

void CalibrationAdjustmentDialog::onDone()
{
    // Store the current position (raw servo pulse widths)
    // so we can use it for calibration

    double b, bus, s, sus, e, eus, w, wus, g, gus;

    m_robocon->getPositionAngles(b, bus, s, sus, e, eus, w, wus, g, gus);

    m_base.us = bus;
    m_shoulder.us = sus;
    m_elbow.us = eus;
    m_wrist.us = wus;

    accept();
}

void CalibrationAdjustmentDialog::getCalibration(Angle& base, Angle& shoulder, Angle& elbow, Angle& wrist)
{
    base     = m_base;
    shoulder = m_shoulder;
    elbow    = m_elbow;
    wrist    = m_wrist;
}
