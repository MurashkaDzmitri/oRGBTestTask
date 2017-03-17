#ifndef MATRIXMULTIPLIER_H
#define MATRIXMULTIPLIER_H

#include <QGenericMatrix>

class MatrixMultiplier
{
public:
    MatrixMultiplier();
    static QMatrix2x2 multiplySquare2D(QMatrix2x2 A, QMatrix2x2 B);

    static QMatrix3x3 multiplySquare3D(QMatrix3x3 A, QMatrix3x3 B);
};

#endif // MATRIXMULTIPLIER_H
