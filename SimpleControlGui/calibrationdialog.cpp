#include "calibrationdialog.h"
#include "ui_calibrationdialog.h"

CalibrationDialog::CalibrationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationDialog),
    data(new CalibrationData),
    home(new HomePosition)
{
    ui->setupUi(this);

    loadData();
}

CalibrationDialog::~CalibrationDialog()
{
    delete ui;
}

void CalibrationDialog::loadData()
{
    data->loadCalibrationData();

    CalibrationAngle base, shoulder, elbow, wrist, gripper;

    data->getCalibrationData(base, shoulder, elbow, wrist, gripper);

    ui->baseMinAngle->setText(QString("%1").arg(base.minDeg));
    ui->baseMinUs->setText(QString("%1").arg(base.minUs));
    ui->baseMaxAngle->setText(QString("%1").arg(base.maxDeg));
    ui->baseMaxUs->setText(QString("%1").arg(base.maxUs));

    ui->shoulderMinAngle->setText(QString("%1").arg(shoulder.minDeg));
    ui->shoulderMinUs->setText(QString("%1").arg(shoulder.minUs));
    ui->shoulderMaxAngle->setText(QString("%1").arg(shoulder.maxDeg));
    ui->shoulderMaxUs->setText(QString("%1").arg(shoulder.maxUs));

    ui->elbowMinAngle->setText(QString("%1").arg(elbow.minDeg));
    ui->elbowMinUs->setText(QString("%1").arg(elbow.minUs));
    ui->elbowMaxAngle->setText(QString("%1").arg(elbow.maxDeg));
    ui->elbowMaxUs->setText(QString("%1").arg(elbow.maxUs));

    ui->wristMinAngle->setText(QString("%1").arg(wrist.minDeg));
    ui->wristMinUs->setText(QString("%1").arg(wrist.minUs));
    ui->wristMaxAngle->setText(QString("%1").arg(wrist.maxDeg));
    ui->wristMaxUs->setText(QString("%1").arg(wrist.maxUs));

    ui->gripperMinAngle->setText(QString("%1").arg(gripper.minDeg));
    ui->gripperMinUs->setText(QString("%1").arg(gripper.minUs));
    ui->gripperMaxAngle->setText(QString("%1").arg(gripper.maxDeg));
    ui->gripperMaxUs->setText(QString("%1").arg(gripper.maxUs));

    home->loadHomePosition();

    double hbase, hshoulder, helbow, hwrist, hgripper;

    home->getHomeAngles(hbase, hshoulder, helbow, hwrist, hgripper);

    ui->homingBase->setValue(hbase);
    ui->homingShoulder->setValue(hshoulder);
    ui->homingElbow->setValue(helbow);
    ui->homingWrist->setValue(hwrist);
    ui->homingGripper->setValue(hgripper);

}

void CalibrationDialog::accept()
{

    CalibrationAngle base, shoulder, elbow, wrist, gripper;

    base.minDeg = atof(ui->baseMinAngle->text().toStdString().c_str());
    base.maxDeg = atof(ui->baseMaxAngle->text().toStdString().c_str());
    base.minUs = atof(ui->baseMinUs->text().toStdString().c_str());
    base.maxUs = atof(ui->baseMaxUs->text().toStdString().c_str());

    shoulder.minDeg = atof(ui->shoulderMinAngle->text().toStdString().c_str());
    shoulder.maxDeg = atof(ui->shoulderMaxAngle->text().toStdString().c_str());
    shoulder.minUs = atof(ui->shoulderMinUs->text().toStdString().c_str());
    shoulder.maxUs = atof(ui->shoulderMaxUs->text().toStdString().c_str());

    elbow.minDeg = atof(ui->elbowMinAngle->text().toStdString().c_str());
    elbow.maxDeg = atof(ui->elbowMaxAngle->text().toStdString().c_str());
    elbow.minUs = atof(ui->elbowMinUs->text().toStdString().c_str());
    elbow.maxUs = atof(ui->elbowMaxUs->text().toStdString().c_str());

    wrist.minDeg = atof(ui->wristMinAngle->text().toStdString().c_str());
    wrist.maxDeg = atof(ui->wristMaxAngle->text().toStdString().c_str());
    wrist.minUs = atof(ui->wristMinUs->text().toStdString().c_str());
    wrist.maxUs = atof(ui->wristMaxUs->text().toStdString().c_str());

    gripper.minDeg = atof(ui->gripperMinAngle->text().toStdString().c_str());
    gripper.maxDeg = atof(ui->gripperMaxAngle->text().toStdString().c_str());
    gripper.minUs = atof(ui->gripperMinUs->text().toStdString().c_str());
    gripper.maxUs = atof(ui->gripperMaxUs->text().toStdString().c_str());

    data->updateCalibrationData(base, shoulder, elbow, wrist, gripper);
    data->saveCalibrationData();

    double hbase, hshoulder, helbow, hwrist, hgripper;

    hbase = ui->homingBase->value();
    hshoulder = ui->homingShoulder->value();
    helbow = ui->homingElbow->value();
    hwrist = ui->homingWrist->value();
    hgripper = ui->homingGripper->value();

    home->setHomeAngles(hbase, hshoulder, helbow, hwrist, hgripper);
    home->saveHomePosition();

    QDialog::accept();
}

void CalibrationDialog::reject()
{
    loadData();

    QDialog::reject();
}
