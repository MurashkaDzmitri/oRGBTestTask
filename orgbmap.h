#ifndef ORGBMAP_H
#define ORGBMAP_H

#include <QList>
#include <QVector>
#include <QObject>
#include <QVector4D>
#include <QPixmap>
#include <QGenericMatrix>
#include <qmath.h>

#include <matrixmultiplier.h>

typedef QGenericMatrix<3, 1, float> Matrix3x1;
typedef  QGenericMatrix<2, 1, float> Matrix2x1;

class ORGBMap : public QObject
{
    Q_OBJECT

public:
    explicit ORGBMap(QPixmap pixmap, QObject *parent = 0);
    void fromPixmap(QPixmap pixmap);
    void fromImage(QImage image);
    QPixmap toPixmap(float ybDelta = 0.0, float rgDelta = 0.0);
    void checkSafety(QImage image );


signals:

public slots:

private:

    void resize(int rows, int columns);
    void resize(QPoint point);
    void initializeBaseMatrices();

    float addSafeAsCos(float value, float delta);
    float findAbsMax(bool isRG);

    QMatrix3x3 rotateLCCToORGB(QMatrix3x3 matr );
    QMatrix3x3 rotateORGBToLCC(QMatrix3x3 matr );
    QMatrix2x2 generateRotateMatrix(float angle);

    QList<QVector < QMatrix3x3 > > _pixelsORGB;
    QMatrix3x3 _fromRGBToLCC;
    QMatrix3x3 _fromLCCToRGB;



};

#endif // ORGBMAP_H
