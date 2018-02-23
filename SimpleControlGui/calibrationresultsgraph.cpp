#include "calibrationresultsgraph.h"
#include <QPainter>
#include <QCoreApplication>
#include <QtGui>

CalibrationResultsGraph::CalibrationResultsGraph(QWidget *parent) : QWidget(parent)
{
    resize(350, 250);

    setMinimumSize(QSize(350,250));

}

void CalibrationResultsGraph::paintEvent(QPaintEvent* event)
{

    QPainter painter(this);

    QColor segColor(255,0,0);

    painter.setPen(segColor);
    painter.setRenderHint(QPainter::Antialiasing);

    double minAngle = -180.0;
    double maxAngle = 180.0;

    double minUs = 500.0;
    double maxUs = 3000.0;

    std::vector<Angle> vec = {m_a1, m_a2, m_a3, m_a4};

    double xmin = 0.15 * QWidget::width();
    double xmax = 0.9 * QWidget::width();

    double ymin = 0.1 * QWidget::height();
    double ymax = 0.9 * QWidget::height();


    for (const Angle& a : vec)
    {
        double x, y;

        x = xmin + (a.deg - minAngle) / (maxAngle - minAngle) * (xmax - xmin);
        y = ymin + (a.us - minUs) / (maxUs - minUs) * (ymax - ymin);

        painter.drawArc((int)x, (int)y, 10, 10, 0, 360 * 16);

        printf("Angle: %g %g\n", a.deg, a.us);
        fflush(0);

    }

    painter.setPen(QColor(0,0,0));

    painter.drawLine(xmin, ymax, xmax, ymax);
    painter.drawLine(xmin, ymax, xmin, ymin);

    for (int i = 0; i <= 9; i ++)
    {
        int x, y;
        double a;

        x = xmin + (xmax - xmin)*(i / 9.0);
        y = ymax;

        a = minAngle + (maxAngle - minAngle)*(i / 9.0);

        painter.drawLine(x, y, x, y + 10);
        painter.drawText(x - 20, y + 20, QString("%1").arg(a));
    }


    for (int i = 0; i <= 5; i ++)
    {
        int x, y;
        double a;

        y = ymin + (ymax - ymin)*(i / 5.0);
        x = xmin;

        a = minUs + (maxUs - minUs)*(i / 5.0);

        painter.drawLine(x, y, x - 10, y);
        painter.drawText(x - 40, y, QString("%1").arg(a));
    }

}

void CalibrationResultsGraph::setAngles(const Angle& a1, const Angle& a2, const Angle& a3, const Angle& a4)
{
    m_a1 = a1;
    m_a2 = a2;
    m_a3 = a3;
    m_a4 = a4;
}


