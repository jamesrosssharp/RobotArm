#include "calibrationadjustmentdialog.h"
#include "ui_calibrationadjustmentdialog.h"

#include "chessboard.h"
#include "kinematics.h"

const double kXInc = 0.5;
const double kYInc = 0.5;
const double kZInc = 0.5;
const double kWInc = 0.1;

const double kXMove = 5.0;
const double kYMove = 5.0;
const double kZMove = 5.0;


CalibrationAdjustmentDialog::CalibrationAdjustmentDialog(RobotController* robocon,  Chessboard* chessboard, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationAdjustmentDialog),
    m_robocon(robocon),
    m_moveBeforeAdjust(false),
    m_chessboard(chessboard)
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

void CalibrationAdjustmentDialog::doDialog(bool isTrue)
{
    double x, y, z;

    if (isTrue)
        m_chessboard->getTrueSquareXYZ(m_rank, m_file, x, y, z);
    else
        m_chessboard->getSquareXYZ(m_rank, m_file, x, y, z);

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));
    ui->wrist->setText(QString("%1").arg(m_robocon->getWristAdjust()));

    m_robocon->computeInverseCartesian(x, y, z, m_base.deg, m_shoulder.deg, m_elbow.deg, m_wrist.deg);

    show();
}

void CalibrationAdjustmentDialog::onXMinus()
{
    double x, y, z;


    m_robocon->getPositionCartesian(x, y, z);

    if (m_moveBeforeAdjust)
        m_robocon->moveToCartesian(x + kXMove, y + kYMove, z + kZMove);


    x -= kXInc;

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));

}

void CalibrationAdjustmentDialog::onXPlus()
{
    double x, y, z;

    m_robocon->getPositionCartesian(x, y, z);

    if (m_moveBeforeAdjust)
        m_robocon->moveToCartesian(x + kXMove, y + kYMove, z + kZMove);


    x += kXInc;

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));
}

void CalibrationAdjustmentDialog::onYMinus()
{
    double x, y, z;

    m_robocon->getPositionCartesian(x, y, z);
    if (m_moveBeforeAdjust)
        m_robocon->moveToCartesian(x + kXMove, y + kYMove, z + kZMove);

    y -= kYInc;

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));
}

void CalibrationAdjustmentDialog::onYPlus()
{
    double x, y, z;

    m_robocon->getPositionCartesian(x, y, z);
    if (m_moveBeforeAdjust)
        m_robocon->moveToCartesian(x + kXMove, y + kYMove, z + kZMove);

    y += kXInc;

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));
}

void CalibrationAdjustmentDialog::onZMinus()
{
    double x, y, z;

    m_robocon->getPositionCartesian(x, y, z);
    if (m_moveBeforeAdjust)
        m_robocon->moveToCartesian(x + kXMove, y + kYMove, z + kZMove);

    z -= kZInc;

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));
}

void CalibrationAdjustmentDialog::onZPlus()
{
    double x, y, z;

    m_robocon->getPositionCartesian(x, y, z);
    if (m_moveBeforeAdjust)
        m_robocon->moveToCartesian(x + kXMove, y + kYMove, z + kZMove);

    z += kZInc;

    m_robocon->moveToCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));
}

void CalibrationAdjustmentDialog::onWristPlus()
{

    double w = m_robocon->getWristAdjust();
    w += kWInc;
    m_robocon->setWristAdjust(w);

    ui->wrist->setText(QString("%1").arg(m_robocon->getWristAdjust()));

}

void CalibrationAdjustmentDialog::onWristMinus()
{
    double w = m_robocon->getWristAdjust();
    w -= kWInc;
    m_robocon->setWristAdjust(w);

    ui->wrist->setText(QString("%1").arg(m_robocon->getWristAdjust()));


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
