#ifndef CHESSBOARD_H
#define CHESSBOARD_H


class Chessboard
{
public:
    Chessboard();

    void getSquareXYZ(int rank, int file, double& x, double& y, double& z);
    void getTrueSquareXYZ(int rank, int file, double& x, double& y, double& z);
    void setSquareCalibration(int rank, int file, double x, double y, double z);

    void load();
    void save();
private:

    void setDefaults();

    double m_x[8][8];
    double m_y[8][8];
    double m_z[8][8];


};

#endif // CHESSBOARD_H
