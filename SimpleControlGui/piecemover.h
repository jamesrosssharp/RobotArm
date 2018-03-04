#pragma once

#include <string>

#include "robotcontroller.h"
#include "chessboard.h"

class PieceMover
{
public:
    PieceMover(RobotController* robocon, Chessboard* chessboard);

    void begin(int rankFrom, int fileFrom, int rankTo, int fileTo, std::string piece);

    void onMoveComplete();

private:

    void processState();
    void openGripper();
    void closeGripper();
    void moveUp();
    void moveDown(int rank, int file, double adjust = 0.0);

    enum class State
    {
        kIdle,
        kHome1,
        kMoveUp1,
        kMoveTo1,
        kOpenGripper1,
        kMoveDown1,
        kCloseGripper1,
        kMoveUp2,
        kMoveTo2,
        kMoveDown2,
        kOpenGripper2,
        kMoveUp3,
        kHome2
    };


    State            m_state;
    RobotController* m_robocon;
    Chessboard*      m_chessboard;

    int m_rankFrom;
    int m_rankTo;
    int m_fileFrom;
    int m_fileTo;
    std::string m_piece;

};
