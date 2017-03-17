#include "matrixmultiplier.h"

MatrixMultiplier::MatrixMultiplier()
{

}

QMatrix2x2 MatrixMultiplier::multiplySquare2D(QMatrix2x2 A, QMatrix2x2 B)
{
    int size = 2;
    QMatrix2x2 result; result.fill(0);
    for (int i=0; i < size; i++)
    {
        for (int j=0; j < size; j++)
        {
            for (int k=0; k < size; k++)
            {
                result(j, i) +=   B(j , k) * A(k, i);
            }
//            if (qAbs(result(j, i)) - 0 < 0.001)
//            {
//                result(j, i) = 0;
//            }
        }

    }
    return result;
}

QMatrix3x3 MatrixMultiplier::multiplySquare3D(QMatrix3x3 A, QMatrix3x3 B)
{
    int size = 3;
    QMatrix3x3 result; result.fill(0);
    for (int i=0; i < size; i++)
    {
        for (int j=0; j < size; j++)
        {
            for (int k=0; k < size; k++)
            {
                result(j, i) +=   B(j , k) * A(k, i);
            }
//            if (qAbs(result(j, i)) - 0 < 0.001)
//            {
//                result(j, i) = 0;
//            }
        }

    }

    return result;
}
