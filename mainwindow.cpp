#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QFileDialog>
#include "./filter/filter_difference.h"
#include "./color/color_colorspace.h"
using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->image->setMouseTracking(true);
    QAction *actionopen = ui->actionopen;
    actionopen->setShortcuts(QKeySequence::Open);
    actionopen->setStatusTip(tr("Create a new file"));
    connect(actionopen, &QAction::triggered, this, &MainWindow::open);
    QAction *actionsaveas = ui->actionsave_as;
    actionsaveas->setShortcuts(QKeySequence::SaveAs);
    actionsaveas->setStatusTip(tr("Save as"));
    connect(actionsaveas, &QAction::triggered, this, &MainWindow::saveas);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->image->pushSave(ui->lineEdit->text());
}

void MainWindow::open()
{
    qDebug() << "open" << endl;
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/jimmy", tr("Image Files (*.png *.jpg *.bmp)"));
    ui->image->showImage(imread(filename.toStdString()));
}

void MainWindow::saveas()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), "/home/jimmy", tr("Image Files (*.png *.jpg *.bmp)"));
    // TODO
}

void MainWindow::on_buttonOpen_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/jimmy", tr("Image Files (*.png *.jpg *.bmp)"));
    qDebug() << filename << endl;
    if (filename.toStdString() == "")
        return;
    Mat img = imread(filename.toStdString());
    ui->image->showImage(img);
    ui->history->addImg(img, QString("Load"));
}

void MainWindow::on_buttonSaveAs_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), "/home/jimmy", tr("Image Files (*.png *.jpg *.bmp)"));
    if (filename.toStdString() == "")
        return;
    Mat img;
    ui->image->getImage(img);
    imwrite(filename.toStdString(), img);
}

void MainWindow::on_buttonCanay_clicked()
{
    int value;
    bool accept = Dialog().getChoice(value);
    qDebug() << accept << value << endl;
}

void MainWindow::on_buttonSobel_clicked()
{
    Mat img;
    ui->image->getImage(img);
    if (img.channels() == 3)
        img = RGBToGray<uchar>(AVG).doMap(img);
    img = SobelFilter().doFilter(img);
    img.convertTo(img, CV_8UC1, 0.25, 0);
    ui->image->showImage(img);
    ui->history->addImg(img, QString("Sobel"));
}

void MainWindow::on_buttonToGrayscale_clicked()
{
    Mat img;
    ui->image->getImage(img);
    if (img.channels() != 3)
        return;
    img = RGBToGray<uchar>(AVG).doMap(img);
    ui->image->showImage(img);
    ui->history->addImg(img, QString("To Grayscale"));
}
