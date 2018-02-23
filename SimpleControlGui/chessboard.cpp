#include "chessboard.h"

void Chessboard::getSquareXYZ(int rank, int file, double &x, double &y, double &z)
{
    const double gridSpacing = 90.0 / 7.0;
    const double gridY = 85.0;

    x =  -45.0 + gridSpacing * file;
    y = (7 - rank) * gridSpacing + gridY;
    z = 15.0 + 10.0;

}
