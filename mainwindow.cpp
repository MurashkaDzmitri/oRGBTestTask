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
    //map.checkSafety(pixmap.toImage());
    ui->label_0->setPixmap(map.toPixmap(1, 1).scaled(ui->label_0->size(),Qt::KeepAspectRatio));
    if (pixmap.toImage() == map.toPixmap(1, 1).toImage() )
    {
        qDebug() << "Images are equal!!";
    }
}

void MainWindow::setUpImagesLabels()
{
//    QObjectList listLabels = ui->imagesWidget->children()
}

