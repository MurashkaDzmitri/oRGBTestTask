#include "orgbmap.h"

ORGBMap::ORGBMap(QPixmap pixmap, QObject *parent)
    : QObject(parent),
      _sourceMap(pixmap)
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
//    float maxYB = findAbsMax(false);
//    float maxRG = findAbsMax(true);
//    ybDelta = maxYB / addSafeAsCos(maxYB, ybDelta);
//    rgDelta = maxRG / addSafeAsCos(maxRG, rgDelta);
    for (int i = 0; i < _pixelsORGB.count(); i++)
    {
        for (int j = 0; j < _pixelsORGB[i].length(); j++)
        {
            resultImage.setPixelColor(j, i, pixelFromORGBToRGB(_pixelsORGB[i][j], ybDelta, rgDelta) );
//            if (resultImage.pixelColor(j, i) != _sourceMap.toImage().pixelColor(j, i))
//            {
//                qDebug() << "pixel not equal";
//            }
        }
    }
    return QPixmap::fromImage(resultImage);
}

void ORGBMap::checkSafety(QImage image) // Something like test function and not refactored
{
    for (int i = 0; i < image.height(); i++)
    {
        for (int j = 0; j < image.width(); j++ )
        {
            if (i == 153 && j == 256)
            {
                qDebug() << i << j << "First bad pixel";
            }
            QColor colorPixel(image.pixel(j, i));
            QMatrix3x3 pixelMatr; pixelMatr.fill(0.);
            pixelMatr.data()[0] = qPow(colorPixel.redF()  , (1./2.2));
            pixelMatr.data()[3] = qPow(colorPixel.greenF(), (1./2.2)) ;
            pixelMatr.data()[6] = qPow(colorPixel.blueF() , (1./2.2));
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

            color.setRedF  (qPow(currentMatrix.data()[0],1./2.2));
            color.setGreenF(qPow(currentMatrix.data()[3],1./2.2));
            color.setBlueF (qPow(currentMatrix.data()[6],1./2.2));
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
                            0.5     , 0.5   , -1.,
                            0.8660, -0.8660, 0.};
    _fromRGBToLCC = QMatrix3x3(valuesRGBToLCC);
    _fromRGBToLCC = _fromRGBToLCC.transposed();
    float valuesLCCToRGB[] = {1.    , 0.1140, 322./433.,
                              1.    , 0.1140, -(178./433.),
                              1.    , -0.8860, 72./433.};
    _fromLCCToRGB = QMatrix3x3(valuesLCCToRGB);
    _fromLCCToRGB = _fromLCCToRGB.transposed();
}

QMatrix3x3 ORGBMap::rotateLCCToORGB(QMatrix3x3 matr)
{
    double o = qAtan2(matr.data()[6] , matr.data()[3]);
    double o_ = o;
    if (o_ < M_PI / 3.)
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
        o_ = M_PI/3. + (4./3.)*(o_ - M_PI/2.);
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

QColor ORGBMap::pixelFromORGBToRGB(QMatrix3x3 pixel, float ybDelta, float rgDelta)
{
    pixel.data()[3] = addSafeAsCos(pixel.data()[3], ybDelta);
    pixel.data()[6] = addSafeAsCos(pixel.data()[6], rgDelta);
    pixel = rotateORGBToLCC(pixel);
    pixel =   MatrixMultiplier::multiplySquare3D( _fromLCCToRGB , pixel) ;
    QColor color;
    color.setRedF( toValidRGBValue( qPow(pixel.data()[0], 2.2)));
    color.setGreenF(toValidRGBValue( qPow(pixel.data()[3], 2.2)));
    color.setBlueF( toValidRGBValue( qPow(pixel.data()[6], 2.2)));
    return color;
}

QMatrix3x3 ORGBMap::pixelFromRGBToORGB(QColor pixel)
{
    QMatrix3x3 pixelMatr; pixelMatr.fill(0.);
    pixelMatr.data()[0] = qPow( pixel.redF(), (1./2.2));
    pixelMatr.data()[3] = qPow( pixel.greenF(), (1./2.2));
    pixelMatr.data()[6] = qPow( pixel.blueF(), (1./2.2));
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

float ORGBMap::toValidRGBValue(float value)
{
    if (value < 0 )
        value = 0;
    else if (value > 1)
        value = 1;
    return value;
}
