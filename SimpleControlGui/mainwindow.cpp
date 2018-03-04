#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "chessboard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cal(new CalibrationDialog(this)),
    m_chessboard(new Chessboard)
{
    ui->setupUi(this);
    robocon = new RobotController(cal->getCalData(), cal->getHomePosition());
    calwiz = new CalibrationWizard(this, robocon, cal->getCalData(), m_chessboard);

    m_sqcal = new SquareCalibrator(m_chessboard, robocon, this);
    m_pieceMover = new PieceMover(robocon, m_chessboard);

    connect(cal, SIGNAL(accepted()), this, SLOT(onCalibrationDialogClosed()));

    timer = new QTimer();

    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

MainWindow::~MainWindow()
{
    timer->stop();
    delete timer;
    // cal is child; implicitly deleted by Qt
    delete robocon;
    delete calwiz;
    delete m_chessboard;
    delete m_sqcal;
    delete ui;
}

void MainWindow::showCalibrationWindow()
{
    cal->show();
}

void MainWindow::onCalibrationDialogClosed()
{
    updateSecondaryWidgets();
}

bool MainWindow::init()
{
    // first initialise joint angles
    robocon->home();

    onSpeedChanged();

    // initialise serial connection
    robocon->init();

    // fill in initial GUI values
    updatePrimaryWidgets();
    updateSecondaryWidgets();

    // create a timer to update UI and robocon every 20 ms

    timer->start(50);
}


void MainWindow::updatePrimaryWidgets()
{
    double baseAng, baseUs;
    double shoulderAng, shoulderUs;
    double elbowAng, elbowUs;
    double wristAng, wristUs;
    double gripperAng, gripperUs;

    robocon->getPositionAngles(baseAng, baseUs, shoulderAng, shoulderUs,
                               elbowAng, elbowUs, wristAng, wristUs, gripperAng, gripperUs);

    ui->baseAng->setValue(baseAng);
    ui->shoulderAng->setValue(shoulderAng);
    ui->elbowAng->setValue(elbowAng);
    ui->wristAng->setValue(wristAng);
    ui->gripperAng->setValue(gripperAng);

    double rho, theta, h;

    robocon->getPositionCylindrical(rho, theta, h);

    ui->rhoIn->setValue(rho);
    ui->thetaIn->setValue(theta);
    ui->hIn->setValue(h);

    double x, y, z;

    robocon->getPositionCartesian(x, y, z);

    ui->xin->setValue(x);
    ui->yin->setValue(y);
    ui->zin->setValue(z);

}

void MainWindow::updateSecondaryWidgets()
{
    double baseAng, baseUs;
    double shoulderAng, shoulderUs;
    double elbowAng, elbowUs;
    double wristAng, wristUs;
    double gripperAng, gripperUs;

    robocon->getPositionAngles(baseAng, baseUs, shoulderAng, shoulderUs,
                               elbowAng, elbowUs, wristAng, wristUs, gripperAng, gripperUs);

    ui->baseUs->setText(QString("%1").arg(baseUs));
    ui->shoulderUs->setText(QString("%1").arg(shoulderUs));
    ui->elbowUs->setText(QString("%1").arg(elbowUs));
    ui->wristUs->setText(QString("%1").arg(wristUs));
    ui->gripperUs->setText(QString("%1").arg(gripperUs));

    double rho, theta, h;

    robocon->getPositionCylindrical(rho, theta, h);

    ui->rho->setText(QString("%1").arg(rho));
    ui->theta->setText(QString("%1").arg(theta));
    ui->h->setText(QString("%1").arg(h));

    double x, y, z;

    robocon->getPositionCartesian(x, y, z);

    ui->x->setText(QString("%1").arg(x));
    ui->y->setText(QString("%1").arg(y));
    ui->z->setText(QString("%1").arg(z));

}

void MainWindow::update()
{
    RobotController::ReturnCode ret = robocon->update();

    if (ret == RobotController::ReturnCode::kDone)
    {
        enablePrimaryWidgets();
        updatePrimaryWidgets();
        m_pieceMover->onMoveComplete();
    }
    else if (ret == RobotController::ReturnCode::kMoving)
    {
        disablePrimaryWidgets();
    }
    updateSecondaryWidgets();
}

void MainWindow::disablePrimaryWidgets()
{
    ui->baseAng->setEnabled(false);
    ui->shoulderAng->setEnabled(false);
    ui->elbowAng->setEnabled(false);
    ui->wristAng->setEnabled(false);
    ui->gripperAng->setEnabled(false);

    ui->rhoIn->setEnabled(false);
    ui->thetaIn->setEnabled(false);
    ui->hIn->setEnabled(false);

    ui->xin->setEnabled(false);
    ui->yin->setEnabled(false);
    ui->zin->setEnabled(false);
}

void MainWindow::enablePrimaryWidgets()
{
    ui->baseAng->setEnabled(true);
    ui->shoulderAng->setEnabled(true);
    ui->elbowAng->setEnabled(true);
    ui->wristAng->setEnabled(true);
    ui->gripperAng->setEnabled(true);

    ui->rhoIn->setEnabled(true);
    ui->thetaIn->setEnabled(true);
    ui->hIn->setEnabled(true);

    ui->xin->setEnabled(true);
    ui->yin->setEnabled(true);
    ui->zin->setEnabled(true);
}

void MainWindow::home()
{
    robocon->home();
}

void MainWindow::onAngleChanged()
{
    double baseAng;
    double shoulderAng;
    double elbowAng;
    double wristAng;
    double gripperAng;


    baseAng = ui->baseAng->value();
    shoulderAng = ui->shoulderAng->value();
    elbowAng = ui->elbowAng->value();
    wristAng = ui->wristAng->value();
    gripperAng = ui->gripperAng->value();

    robocon->setJointAngles(baseAng, shoulderAng, elbowAng, wristAng, gripperAng);

}

void MainWindow::onSpeedChanged()
{
    robocon->setSpeed((double)ui->speedSlider->value() / 5.0);
}

void MainWindow::onCartGo()
{

    double x = ui->xin->value();
    double y = ui->yin->value();
    double z = ui->zin->value();

    robocon->moveToCartesian(x, y, z);

}

void MainWindow::onCylGo()
{

    double rho = ui->rhoIn->value();
    double theta = ui->thetaIn->value();
    double h = ui->hIn->value();

    robocon->moveToCylindrical(rho, theta, h);
}

void MainWindow::onGoSquare()
{
    int rank = ui->rankCombo->currentIndex();
    int file = ui->fileCombo->currentIndex();

    std::cout << "Square: " << (char)('a' + file) << rank + 1 << std::endl;

    double x, y, z;
    double xc, yc, zc;


    m_chessboard->getSquareXYZ(rank, file, x, y, z);

    robocon->getPositionCartesian(xc, yc, zc);
    robocon->moveToCartesian(x, y, zc);

}

void MainWindow::onCalibrationWizard()
{
    calwiz->doWizard();
}

void MainWindow::onCalibrateSquare()
{
    int rank = ui->rankCombo->currentIndex();
    int file = ui->fileCombo->currentIndex();

    m_sqcal->doDialog(rank, file);

}

void MainWindow::onMovePiece()
{

    int rank1, file1;
    int rank2, file2;

    rank1 = ui->fromRank->currentIndex();
    file1 = ui->fromFile->currentIndex();
    rank2 = ui->toRank->currentIndex();
    file2 = ui->toFile->currentIndex();

    std::string piece = ui->piece->currentText().toStdString();

    m_pieceMover->begin(rank1, file1, rank2, file2, piece);

}
