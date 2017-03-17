#-------------------------------------------------
#
# Project created by QtCreator 2017-03-14T18:47:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = oRGBTestTask
TEMPLATE = app

CONFIG(debug, debug|release) {
    BUILD_DIR = ../buildORGB/debug
} else {
    BUILD_DIR = ../buildORGB/release
}

DESTDIR = $$BUILD_DIR/bin
OBJECTS_DIR = $$BUILD_DIR/obj
MOC_DIR = $$BUILD_DIR/moc
RCC_DIR = $$BUILD_DIR/rcc
UI_DIR = $$BUILD_DIR/ui



SOURCES += main.cpp\
        mainwindow.cpp \
    orgbmap.cpp \
    matrixmultiplier.cpp

HEADERS  += mainwindow.h \
    orgbmap.h \
    matrixmultiplier.h

FORMS    += mainwindow.ui
