#include "matrixmultiplier.h"

#define MATRIX_MULTIPLY result.fill(0);                                    \
                        for (int i=0; i < size; i++)                       \
                        {                                                  \
                            for (int j=0; j < size; j++)                   \
                            {                                              \
                                for (int k=0; k < size; k++)               \
                                {                                          \
                                    result(j, i) +=   B(j , k) * A(k, i);  \
                                }                                          \
                            }                                              \
                                                                           \
                        }                                                  \
                        return result;


MatrixMultiplier::MatrixMultiplier()
{

}

QMatrix2x2 MatrixMultiplier::multiplySquare2D(QMatrix2x2 A, QMatrix2x2 B)
{
    int size = 2;
    QMatrix2x2 result;
    MATRIX_MULTIPLY

}

QMatrix3x3 MatrixMultiplier::multiplySquare3D(QMatrix3x3 A, QMatrix3x3 B)
{
    int size = 3;
    QMatrix3x3 result; result.fill(0);
    MATRIX_MULTIPLY
}
