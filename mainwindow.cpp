#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QFileDialog>
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
    Mat img = imread(filename.toStdString());
    ui->image->showImage(img);
    ui->history->addImg(img, QString("Load"));
}

void MainWindow::on_buttonSaveAs_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), "/home/jimmy", tr("Image Files (*.png *.jpg *.bmp)"));
    Mat img = ui->image->getImage();
    imwrite(filename.toStdString(), img);
}