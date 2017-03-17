#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect( ui->actionOpen, SIGNAL(triggered(bool)), SLOT(handleOpenFileClicked()));
    setUpImagesLabels();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_coolButton_clicked()
{
    _coolDialog = new QColorDialog();
    _coolDialog->show();
    connect(_coolDialog, &QColorDialog::accepted,[this]{handleCoolDialogEnd(true);});
    connect(_coolDialog, &QColorDialog::rejected,[this]{handleCoolDialogEnd(false);});
}

void MainWindow::handleCoolDialogEnd(bool isOk)
{
    _coolColor = _coolDialog->currentColor();
    delete _coolDialog;
    _coolDialog = nullptr;
}

void MainWindow::on_warmButton_clicked()
{
    _warmDialog = new QColorDialog();
    _warmDialog->show();
    connect(_warmDialog, &QColorDialog::accepted,[this]{handleWarmDialogEnd(true);});
    connect(_warmDialog, &QColorDialog::rejected,[this]{handleWarmDialogEnd(false);});
}

void MainWindow::handleWarmDialogEnd(bool isOk)
{
    _warmColor = _warmDialog->currentColor();
    delete _warmDialog;
    _warmDialog = nullptr;
}

void MainWindow::handleOpenFileClicked()
{
    _openFileDialog = new QFileDialog(this);
    _openFileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    connect(_openFileDialog, &QFileDialog::accepted,[this]{handleFileDialogEnd(true);});
    connect(_openFileDialog, &QFileDialog::rejected,[this]{handleFileDialogEnd(false);});
    _openFileDialog->exec();
}

void MainWindow::handleFileDialogEnd(bool isOk)
{
    if (!isOk)
    {
        return;
    }
    QString fileName = !_openFileDialog->selectedFiles().isEmpty() ? _openFileDialog->selectedFiles().first() : "";
    QPixmap pixmap = QPixmap(fileName);
    ui->label_1->setPixmap(pixmap.scaled(ui->label_0->size(),Qt::KeepAspectRatio));

    ORGBMap map(pixmap);
    map.checkSafety(pixmap.toImage());
//    ui->label_0->setPixmap(map.toPixmap(1.0, 1.0).scaled(ui->label_0->size(),Qt::KeepAspectRatio));
//    if (pixmap.toImage() == map.toPixmap(1.0, 1.0).toImage() )
//    {
//        qDebug() << "Images are equal!!";
//    }
}

void MainWindow::setUpImagesLabels()
{
//    QObjectList listLabels = ui->imagesWidget->children()
}

