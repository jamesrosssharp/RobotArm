#include "calibrationresultsdialog.h"
#include "ui_calibrationresultsdialog.h"


CalibrationResultsDialog::CalibrationResultsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrationResultsDialog)
{
    ui->setupUi(this);

    m_graph = new CalibrationResultsGraph(ui->frame);
    m_graph->show();

}

CalibrationResultsDialog::~CalibrationResultsDialog()
{
    delete ui;
}

void CalibrationResultsDialog::setJoint(QString joint, Angle a1, Angle a2, Angle a3, Angle a4,
                                        double trendline_m, double trendline_c)
{
    ui->joint->setText(joint);
    m_graph->setAngles(a1, a2, a3, a4);
    m_graph->setTrendline(trendline_m, trendline_c);
}

