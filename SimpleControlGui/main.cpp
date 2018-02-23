#include <iostream>

#include <QApplication>

#include "mainwindow.h"

using namespace std;

int main(int argc, char** argv)
{

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("RobotArm control GUI");
    w.init();
    w.show();

    return a.exec();

}

