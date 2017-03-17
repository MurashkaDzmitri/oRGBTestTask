#include <QApplication>

#include "mainwindow.h"
#include "orgbmap.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    ORGBMap *p = new ORGBMap(QPixmap());

    return a.exec();
}