#include "orgbmap.h"

ORGBMap::ORGBMap(QPixmap pixmap, QObject *parent) : QObject(parent)
{
    initializeBaseMatrices();
    fromPixmap(pixmap);
}

void ORGBMap::fromPixmap(QPixmap pixmap)
{
    fromImage(pixmap.toImage());
}

void ORGBMap::fromImage(QImage image)
{
    this->resize(image.height(), image.width());
    for (int i = 0; i < image.height(); i++)
    {
        for (int j = 0; j < image.width(); j++ )
        {
            _pixelsORGB[i][j] = pixelFromRGBToORGB(image.pixel(j, i));
        }
    }
}

QPixmap ORGBMap::toPixmap(float ybDelta, float rgDelta)
{
    if (_pixelsORGB.count() == 0)
        return QPixmap();
    QImage resultImage = QImage( _pixelsORGB.first().length(), _pixelsORGB.count(), QImage::Format_RGB32);
    float maxYB = findAbsMax(false);
    float maxRG = findAbsMax(true);
    ybDelta = maxYB / addSafeAsCos(maxYB, ybDelta);
    rgDelta = maxRG / addSafeAsCos(maxRG, rgDelta);
    for (int i = 0; i < _pixelsORGB.count(); i++)
    {
        for (int j = 0; j < _pixelsORGB[i].length(); j++)
        {
            resultImage.setPixelColor(j, i, pixelFromORGBToRGB(_pixelsORGB[i][j]) );
        }
    }
    return QPixmap::fromImage(resultImage);
}

void ORGBMap::checkSafety(QImage image) // Something like test function
{
    for (int i = 0; i < image.height(); i++)
    {
        for (int j = 0; j < image.width(); j++ )
        {
            QColor colorPixel(image.pixel(j, i));
            QMatrix3x3 pixelMatr; pixelMatr.fill(0.);
            pixelMatr.data()[0] = colorPixel.redF(); pixelMatr.data()[3] = colorPixel.greenF(); pixelMatr.data()[6] = colorPixel.blueF();
            QMatrix3x3 rgbMatrix = pixelMatr;
            pixelMatr =   MatrixMultiplier::multiplySquare3D( _fromRGBToLCC , pixelMatr) ;
            pixelMatr = rotateLCCToORGB(pixelMatr);

            QMatrix3x3 currentMatrix = pixelMatr;
            currentMatrix = rotateORGBToLCC(currentMatrix);
            currentMatrix =   MatrixMultiplier::multiplySquare3D( _fromLCCToRGB , currentMatrix) ;
            QColor color;

            if ( currentMatrix.data()[0] > 1 ||   currentMatrix.data()[0] < 0 ||
                 currentMatrix.data()[3] > 1 ||   currentMatrix.data()[3] < 0 ||
                 currentMatrix.data()[6] > 1 ||   currentMatrix.data()[6] < 0  )
            {
                qDebug() << "Pixel not valid " << i << j;
            }

            color.setRedF  (currentMatrix.data()[0]);
            color.setGreenF(currentMatrix.data()[3]);
            color.setBlueF (currentMatrix.data()[6]);
        }
    }
}

void ORGBMap::resize(int rows, int columns)
{
    _pixelsORGB.clear();
    for (int i = 0; i < rows; i++)
    {
        QVector<QMatrix3x3 > row;
        row.resize(columns);
        _pixelsORGB.append(row);
    }
}

void ORGBMap::resize(QPoint point)
{
    resize(point.x(), point.y());
}

void ORGBMap::initializeBaseMatrices()
{
    float valuesRGBToLCC[] = {0.2990, 0.5870, 0.1140,
                            0.5000, 0.5000, -1.0000,
                            0.8660, -0.8660, 0.0000};
    _fromRGBToLCC = QMatrix3x3(valuesRGBToLCC);
    float valuesLCCToRGB[] = {1.0000, 0.1140, 0.7436,
                              1.0000, 0.1140, -0.4111,
                              1.0000, -0.8860, 0.1663};
    _fromLCCToRGB = QMatrix3x3(valuesLCCToRGB);
}

QMatrix3x3 ORGBMap::rotateLCCToORGB(QMatrix3x3 matr)
{
    double o = qAtan2(matr.data()[6] , matr.data()[3]);
    double o_ = o;
    if (o_ < M_PI / 3)
    {
        o_*=1.5;
    }
    else if (o_ <= M_PI && o_ >= M_PI/3.)
    {
        o_ = M_PI/2. + (0.75)*(o_ - M_PI/3.);
    }
    QMatrix2x2 matrColor; matrColor.fill(0.);
    matrColor.data()[0] = matr.data()[3]; matrColor.data()[2] = matr.data()[6];
    matrColor =  MatrixMultiplier::multiplySquare2D( generateRotateMatrix(o_ - o), matrColor  ) ;
    matr.data()[3] = matrColor.data()[0];
    matr.data()[6] = matrColor.data()[2];
    return matr;
}

QMatrix3x3 ORGBMap::rotateORGBToLCC(QMatrix3x3 matr)
{
    double o = qAtan2(matr.data()[6] , matr.data()[3]);
    double o_ = o;
    if (o_ < M_PI / 2)
    {
        o_*=2./3.;
    }
    else if (o_ <= M_PI && o_ >= M_PI/2.)
    {
        o_ = M_PI/3. + (1.25)*(o_ - M_PI/2.);
    }
    QMatrix2x2 matrColor; matrColor.fill(0.);
    matrColor.data()[0] = matr.data()[3]; matrColor.data()[2] = matr.data()[6];
    matrColor =  MatrixMultiplier::multiplySquare2D( generateRotateMatrix(o_ - o), matrColor  ) ;
    matr.data()[3] = matrColor.data()[0];
    matr.data()[6] = matrColor.data()[2];
    return matr;
}

QMatrix2x2 ORGBMap::generateRotateMatrix(float angle)
{
    QMatrix2x2 resultMatr; resultMatr.fill(0.);
    resultMatr.data()[0] = qCos(angle);
    resultMatr.data()[1] = -qSin(angle);
    resultMatr.data()[2] = qSin(angle);
    resultMatr.data()[3] = qCos(angle);
    return resultMatr;
}

QColor ORGBMap::pixelFromORGBToRGB(QMatrix3x3 pixel)
{
    pixel.data()[3] = addSafeAsCos(pixel.data()[3], ybDelta);
    pixel.data()[6] = addSafeAsCos(pixel.data()[6], rgDelta);
    pixel = rotateORGBToLCC(pixel);
    pixel =   MatrixMultiplier::multiplySquare3D( _fromLCCToRGB , pixel) ;
    QColor color;
    color.setRedF(pixel.data()[0]);
    color.setGreenF(pixel.data()[3]);
    color.setBlueF(pixel.data()[6]);
    return color;
}

QMatrix3x3 ORGBMap::pixelFromRGBToORGB(QColor pixel)
{
    QMatrix3x3 pixelMatr; pixelMatr.fill(0.);
    pixelMatr.data()[0] = pixel.redF(); pixelMatr.data()[3] = pixel.greenF(); pixelMatr.data()[6] = pixel.blueF();
    pixelMatr =   MatrixMultiplier::multiplySquare3D( _fromRGBToLCC , pixelMatr);
    return rotateLCCToORGB(pixelMatr);
}


float ORGBMap::addSafeAsCos(float value, float delta)
{
    value *= delta;
    if (value > 1.0)
    {
        value = 1.0;
    }
    else if (value < -1.0 )
    {
        value = -1.0;
    }
    return value;
}

float ORGBMap::findAbsMax(bool isRG)
{
    int matrixIndex = isRG ? 6 : 3;
    float max = 0;
    for (int i = 0; i < _pixelsORGB.count(); i++)
    {
        for (int j = 0; j < _pixelsORGB[i].length(); j++)
        {
            if (qAbs(_pixelsORGB[i][j].data()[matrixIndex]) > max )
            {
                max = qAbs(_pixelsORGB[i][j].data()[matrixIndex]);
            }
        }
    }
    return max;
}
