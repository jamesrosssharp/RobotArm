#ifndef CALIBRATIONRESULTSGRAPH_H
#define CALIBRATIONRESULTSGRAPH_H

#include "calibrationdata.h"

#include <QWidget>
#include <QLabel>

class CalibrationResultsGraph : public QWidget
{
    Q_OBJECT
public:
    explicit CalibrationResultsGraph(QWidget *parent = 0);

    void setAngles(const Angle& a1, const Angle& a2, const Angle& a3, const Angle& a4);
    void setTrendline(double m, double c);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:

private:

    Angle m_a1, m_a2, m_a3, m_a4;
    double m_m;
    double m_c;

};

#endif // CALIBRATIONRESULTSGRAPH_H
