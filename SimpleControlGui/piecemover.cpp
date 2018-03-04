#include "piecemover.h"

const double kUpHeight = 70.0;
const double kDownHeight = 32.0;

const double kFastSpeed = 10.0;
const double kMediumSpeed = 2.0;
const double kSlowSpeed = 0.5;
const double kGripPieceHeightAdjust = 7.0;
const double kGripPieceHeightAdjust2 = 1.0;

PieceMover::PieceMover(RobotController* robocon, Chessboard* chessboard)    :
    m_robocon(robocon),
    m_chessboard(chessboard)
{

}

void PieceMover::begin(int rankFrom, int fileFrom, int rankTo, int fileTo, std::string piece)
{
    m_rankFrom = rankFrom;
    m_fileFrom = fileFrom;
    m_rankTo = rankTo;
    m_fileTo = fileTo;
    m_piece = piece;

    if (m_rankFrom == m_rankTo && m_fileFrom == m_fileTo)
        return;

    m_state = State::kHome1;
    processState();

}

void PieceMover::onMoveComplete()
{
    // State transition

    switch (m_state)
    {
        case State::kIdle:
            break;
        case State::kHome1:
            m_state = State::kMoveUp1;
            break;
        case State::kMoveUp1:
            m_state = State::kMoveTo1;
            break;
        case State::kMoveTo1:
            m_state = State::kOpenGripper1;
            break;
        case State::kOpenGripper1:
            m_state = State::kMoveDown1;
            break;
        case State::kMoveDown1:
            m_state = State::kCloseGripper1;
            break;
        case State::kCloseGripper1:
            m_state = State::kMoveUp2;
            break;
        case State::kMoveUp2:
            m_state = State::kMoveTo2;
            break;
        case State::kMoveTo2:
            m_state = State::kMoveDown2;
            break;
        case State::kMoveDown2:
            m_state = State::kOpenGripper2;
            break;
        case State::kOpenGripper2:
            m_state = State::kMoveUp3;
            break;
        case State::kMoveUp3:
            m_state = State::kHome2;
            break;
        case State::kHome2:
            m_state = State::kIdle;
            break;
        default:
            break;
    }

    processState();
}

void PieceMover::processState()
{
    // initiate move for state
    switch (m_state)
    {
        case State::kIdle:
            // do nothing.
            break;
        case State::kHome1:
            printf("Homing...");
            m_robocon->setSpeed(kFastSpeed);
            m_robocon->home();
            break;
        case State::kMoveUp1:
        {
            printf("Moving up...");

            moveUp();
            break;
        }
        case State::kMoveTo1:
        {
            printf("Moving to...", m_rankFrom, m_fileFrom);


            double x, y, z;
            m_chessboard->getSquareXYZ(m_rankFrom, m_fileFrom, x, y, z);

            z += 5.0;

            m_robocon->moveToCartesian(x, y, z);
            break;
        }
        case State::kOpenGripper1:
        {
            printf("Open gripper...");
            openGripper();
            break;
        }
        case State::kMoveDown1:
        {
            m_robocon->setSpeed(kSlowSpeed);
            moveDown(m_rankFrom, m_fileFrom);
            break;
        }
        case State::kCloseGripper1:
        {
            m_robocon->setSpeed(kSlowSpeed);
            closeGripper();
            break;
        }
        case State::kMoveUp2:
        {
            m_robocon->setSpeed(kFastSpeed);
            moveUp();
            break;
        }
        case State::kMoveTo2:
        {
            double x, y, z;
            m_chessboard->getSquareXYZ(m_rankTo, m_fileTo, x, y, z);

            z = kUpHeight;

            m_robocon->setSpeed(kFastSpeed);
            m_robocon->moveToCartesian(x, y, z);
            break;
        }
        case State::kMoveDown2:
        {
            m_robocon->setSpeed(kSlowSpeed);
            moveDown(m_rankTo, m_fileTo, kGripPieceHeightAdjust2);
            break;
        }
        case State::kOpenGripper2:
        {
            m_robocon->setSpeed(kSlowSpeed);
            openGripper();
            break;
        }
        case State::kMoveUp3:
        {
            m_robocon->setSpeed(kMediumSpeed);
            moveUp();
            break;
        }
        case State::kHome2:
        {
            m_robocon->setSpeed(kFastSpeed);
            m_robocon->home();
            break;
        }
        default:
            break;
    }
}

void PieceMover::openGripper()
{
    double b, s, e, w, g, bus, sus, eus, wus, gus;
    m_robocon->getPositionAngles(b, bus, s, sus, e, eus, w, wus, g, gus);

    g = -45.0;
    m_robocon->moveTo(b, s, e, w, g);
}

void PieceMover::closeGripper()
{
    double b, s, e, w, g, bus, sus, eus, wus, gus;
    m_robocon->getPositionAngles(b, bus, s, sus, e, eus, w, wus, g, gus);

    g = 10.0;
    m_robocon->moveTo(b, s, e, w, g);
}

void PieceMover::moveUp()
{
    double x, y, z;
    m_robocon->getPositionCartesian(x, y, z);

    z = kUpHeight;
    m_robocon->moveToCartesian(x, y, z);
}

void PieceMover::moveDown(int rank, int file, double adjust)
{

    double x1, y1, z1;
    m_chessboard->getSquareXYZ(rank, file, x1, y1, z1);

    z1 -= kGripPieceHeightAdjust + adjust;

    m_robocon->moveToCartesian(x1, y1, z1);
}
