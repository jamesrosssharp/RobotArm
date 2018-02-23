#ifndef CALIBRATIONRESULTSDIALOG_H
#define CALIBRATIONRESULTSDIALOG_H

#include "calibrationdata.h"
#include "calibrationresultsgraph.h"

#include <QDialog>

namespace Ui {
class CalibrationResultsDialog;
}

class CalibrationResultsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrationResultsDialog(QWidget *parent = 0);
    ~CalibrationResultsDialog();

    void setJoint(QString joint, Angle a1, Angle a2, Angle a3, Angle a4, double trendline_m, double trendline_c);

private:
    Ui::CalibrationResultsDialog *ui;

    CalibrationResultsGraph* m_graph;
};

#endif // CALIBRATIONRESULTSDIALOG_H
