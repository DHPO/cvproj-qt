#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QFileDialog>
#include "./filter/filter_difference.h"
#include "./filter/filter_smooth.h"
#include "./color/color_colorspace.h"
#include "./dialog/confirmdialog.h"
#include "./dialog/valuedialog.h"
#include "./dialog/kerneldialog.h"
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
    qDebug() << ConfirmDialog().show("test") << endl;
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

void MainWindow::on_buttonLaplace_clicked()
{
    Mat img;
    ui->image->getImage(img);
    if (img.channels() == 3)
        img = RGBToGray<uchar>(AVG).doMap(img);
    double sigma;
    if (ValueDialog().show("sigma", sigma)) {
        img = LaplacianFilter(sigma).doFilter(img);
        img.convertTo(img, CV_8UC1, 1, 0);
        ui->image->showImage(img);
        ui->history->addImg(img, QString("Laplace"));
    }
}

void MainWindow::on_buttonGauss_clicked()
{
    Mat img;
    ui->image->getImage(img);
    double sigma;
    if (ValueDialog().show("sigma", sigma)) {
        img = GaussianFilter(sigma).doFilter(img);
        img.convertTo(img, CV_8UC(img.channels()));
        ui->image->showImage(img);
        ui->history->addImg(img, QString("Gauss"));
    }
}

void MainWindow::on_buttonMean_clicked()
{
    Mat img;
    ui->image->getImage(img);
    double size;
    if (ValueDialog().show("size", size, true)) {
        img = MeanFilter(size).doFilter(img);
        img.convertTo(img, CV_8UC(img.channels()));
        ui->image->showImage(img);
        ui->history->addImg(img, QString("Mean"));
    }
}

void MainWindow::on_buttonCustom_clicked()
{
    Mat kernel;

    if (!KernelDialog().show(kernel))
        return;

    Mat img;
    ui->image->getImage(img);

    if (img.channels() == 3)
        kernel = GrayToRGB<float>().doMap(kernel);

    img = conv(img, kernel, MIRROR);
    img.convertTo(img, CV_8UC(img.channels()));
    ui->image->showImage(img);
    ui->history->addImg(img, QString("Custom Conv"));
}

void MainWindow::on_buttonMedium_clicked()
{
    Mat img;
    ui->image->getImage(img);

    double size;
    if (!ValueDialog().show("size", size, true))
        return;

    img = MediumFilter(size).doFilter(img);
    ui->image->showImage(img);
    ui->history->addImg(img, "Medium");
}
