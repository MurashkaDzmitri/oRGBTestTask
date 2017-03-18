#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include <QRgb>
#include <QFileDialog>

#include "orgbmap.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void handleOpenFileClicked();
    void handleFileDialogEnd(bool isOk = true);

private:
    void setUpImagesLabels();

    Ui::MainWindow *ui;


    QFileDialog *_openFileDialog = nullptr;



};

#endif // MAINWINDOW_H
