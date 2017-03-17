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
    void on_coolButton_clicked();
    void handleCoolDialogEnd(bool isOk = true);

    void on_warmButton_clicked();
    void handleWarmDialogEnd(bool isOk = true);
    void handleOpenFileClicked();
    void handleFileDialogEnd(bool isOk = true);

private:
    void setUpImagesLabels();

    Ui::MainWindow *ui;

    QColorDialog *_coolDialog = nullptr;
    QColorDialog *_warmDialog = nullptr;

    QColor _coolColor;
    QColor _warmColor;

    QFileDialog *_openFileDialog = nullptr;



};

#endif // MAINWINDOW_H
