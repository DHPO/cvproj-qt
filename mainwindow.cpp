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
#include "./morphology/morphology_basic.h"
#include "./dialog/choicedialog.h"
#include "./matrix/matrix_map.h"
#include "./dialog/messagedialog.h"
#include "./dialog/histogramdialog.h"
#include "./color/color_histogram.h"
#include "./dialog/hsvdialog.h"
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
    connect(actionopen, &QAction::triggered, this, &MainWindow::openFile);
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
    Mat img;
    ui->image->getImage(img);
    ui->register_2->addImg(img, ui->lineEdit->text());
}

void MainWindow::openFile()
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
    double sigma;
    if (!ValueDialog().show("sigma", sigma))
        return;

    Mat img;
    ui->image->getImage(img);
    if (img.channels() == 3)
        img = RGBToGray<uchar>(AVG).doMap(img);

    CannyFilter canny(img);
    img = canny.step1(sigma);
    ui->image->showImage(img);

    int tlow, thigh;
    if (!HistogramDialog().show(img, true, true, tlow))
        return;
    if (!HistogramDialog().show(img, true, true, thigh))
        return;
    if (tlow > thigh) {
        tlow = tlow ^ thigh;
        thigh = tlow ^ thigh;
        tlow = tlow ^ thigh;
    }

    canny.setThreshold(thigh, tlow);
    img = canny.step2();

    ui->image->showImage(img);
    ui->history->addImg(img, QString::fromStdString("Canny"));
}

void MainWindow::on_buttonSobel_clicked()
{
    Mat img;
    ui->image->getImage(img);
    if (img.channels() == 3)
        img = RGBToGray<uchar>(AVG).doMap(img);
    img = SobelFilter().doFilter(img);
//    img.convertTo(img, CV_8UC1, 0.25, 0);
    ui->image->showImage(img);
    ui->history->addImg(img, QString("Sobel"));
}

void MainWindow::on_buttonToGrayscale_clicked()
{
    Mat img;
    ui->image->getImage(img);
    if (img.channels() != 3)
        return;
    int choice;
    vector<string> methods({"Red", "Green", "Blue", "Average"});
    ConvertMethod m[] = {RED, GREEN, BLUE, AVG};
    ChoiceDialog().show("method", methods, choice);
    img = RGBToGray<uchar>(m[choice]).doMap(img);
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
//        img.convertTo(img, CV_8UC1, 1, 0);
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
//        img.convertTo(img, CV_8UC(img.channels()));
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
//        img.convertTo(img, CV_8UC(img.channels()));
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
        kernel = GrayToRGB<float>(AVG).doMap(kernel);

    img = conv(img, kernel, MIRROR);
//    img.convertTo(img, CV_8UC(img.channels()));
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

void MainWindow::on_buttonToBinary_clicked()
{
    Mat img;
    ui->image->getImage(img);

    int threshold;
    if (!HistogramDialog().show(img, true, false, threshold))
        return;

    img = GrayToBinary(threshold).doMap(img);

    ui->image->showImage(img);
    ui->history->addImg(img, "To Binary");
}

void MainWindow::on_buttonDilate_clicked()
{
    Mat kernel;
    if (!KernelDialog().show(kernel))
        return;
    kernel.convertTo(kernel, CV_16SC1);

    Mat img;
    ui->image->getImage(img);

    bool binary = ui->checkBox->isChecked();
    if (binary)
        img = dilate_b(img, kernel);
    else
        img = dilate(img, kernel);

    ui->image->showImage(img);
    ui->history->addImg(img, "Dilate");
}

void MainWindow::on_buttonErode_clicked()
{
    Mat kernel;
    if (!KernelDialog().show(kernel))
        return;

    kernel.convertTo(kernel, CV_16SC1);

    Mat img;
    ui->image->getImage(img);

    bool binary = ui->checkBox->isChecked();
    if (binary)
        img = erode_b(img, kernel);
    else
        img = erode(img, kernel);

    ui->image->showImage(img);
    ui->history->addImg(img, "Erode");
}

void MainWindow::on_buttonOpen_2_clicked()
{
    Mat kernel;
    if (!KernelDialog().show(kernel))
        return;

    kernel.convertTo(kernel, CV_16SC1);

    Mat img;
    ui->image->getImage(img);

    bool binary = ui->checkBox->isChecked();
    if (binary)
        img = open_b(img, kernel);
    else
        img = open_g(img, kernel);

    ui->image->showImage(img);
    ui->history->addImg(img, "Open");
}

void MainWindow::on_buttonClose_clicked()
{
    Mat kernel;
    if (!KernelDialog().show(kernel))
        return;

    kernel.convertTo(kernel, CV_16SC1);

    Mat img;
    ui->image->getImage(img);

    bool binary = ui->checkBox->isChecked();
    if (binary)
        img = close_b(img, kernel);
    else
        img = close_g(img, kernel);

    ui->image->showImage(img);
    ui->history->addImg(img, "Close");
}

void MainWindow::on_buttonGrad_clicked()
{
    Mat kernel;
    if (!KernelDialog().show(kernel))
        return;

    kernel.convertTo(kernel, CV_16SC1);

    Mat img;
    ui->image->getImage(img);

    bool binary = ui->checkBox->isChecked();
    if (binary)
        img = morphGrad_b(img, kernel);
    else
        img = morphGrad(img, kernel);

    ui->image->showImage(img);
    ui->history->addImg(img, "Gradiant");
}

void MainWindow::on_buttonTophat_clicked()
{
    Mat kernel;
    if (!KernelDialog().show(kernel))
        return;

    kernel.convertTo(kernel, CV_16SC1);

    Mat img;
    ui->image->getImage(img);

    bool binary = ui->checkBox->isChecked();
    if (binary)
        img = tophat_b(img, kernel);
    else
        img = tophat(img, kernel);

    ui->image->showImage(img);
    ui->history->addImg(img, "Tophat");
}

void MainWindow::on_buttonBlackhat_clicked()
{
    Mat kernel;
    if (!KernelDialog().show(kernel))
        return;

    kernel.convertTo(kernel, CV_16SC1);

    Mat img;
    ui->image->getImage(img);

    bool binary = ui->checkBox->isChecked();
    if (binary)
        img = blackhat_b(img, kernel);
    else
        img = blackhat(img, kernel);

    ui->image->showImage(img);
    ui->history->addImg(img, "Blackhat");
}

 template<int cn>
    class Scale: public MatTransformmer<uchar, cn>
    {
    private:
        double scale;
    public:
        Scale(double scale):scale(scale){};
        Point2f pointMap(Point2i point) override {return Point2f(point.x * scale, point.y * scale);}
        Point2f pointMapReverse(Point2i point) override {return Point2f(point.x / scale, point.y / scale);}
    };


void MainWindow::on_buttonScale_clicked()
{
    double scale;
    if (!ValueDialog().show("Scale", scale))
        return;

    Mat img;
    ui->image->getImage(img);

    if (img.channels() == 1)
        img = Scale<1>(scale).doTrans(img);
    else if (img.channels() == 3)
        img = Scale<3>(scale).doTrans(img);
    else
        return;

    ui->image->showImage(img);
    ui->history->addImg(img, "Scale");
}

template<int cn>
class Rotate: public MatTransformmer<uchar, cn>
{
private:
    double degree;
    double sind;
    double cosd;
public:
    Rotate(double degree):degree(degree){
        sind = sin(degree / 180 * 3.14159);
        cosd = cos(degree / 180 * 3.14159);
    };
    Point2f pointMap(Point2i point) override {
        return Point2f(cosd * point.x - sind * point.y, sind * point.x + cosd * point.y);
    }
    Point2f pointMapReverse(Point2i point) override {
        return Point2f(cosd * point.x + sind * point.y, -sind * point.x + cosd * point.y);
    }
};

void MainWindow::on_buttonRotate_clicked()
{
    double degree;
    if (!ValueDialog().show("Degree", degree))
        return;

    Mat img;
    ui->image->getImage(img);

    if (img.channels() == 1)
        img = Rotate<1>(degree).doTrans(img);
    else if (img.channels() == 3)
        img = Rotate<3>(degree).doTrans(img);
    else
        return;

    ui->image->showImage(img);
    ui->history->addImg(img, "Rotate");
}

template<int cn>
class ShearX: public MatTransformmer<uchar, cn>
{
private:
    double degree;
    double tand;
public:
    ShearX(double degree):degree(degree){
        tand = tan(degree / 180 * 3.14159);
    }
    Point2f pointMap(Point2i point){
        return Point2f(point.x - point.y * tand, point.y);
    }
    Point2f pointMapReverse(Point2i point){
        return Point2f(point.x + point.y * tand, point.y);
    }
};

void MainWindow::on_buttonShearX_clicked()
{
    double degree;
    if (!ValueDialog().show("Degree", degree))
        return;

    Mat img;
    ui->image->getImage(img);

    if (img.channels() == 1)
        img = ShearX<1>(degree).doTrans(img);
    else if (img.channels() == 3)
        img = ShearX<3>(degree).doTrans(img);
    else
        return;

    ui->image->showImage(img);
    ui->history->addImg(img, "ShearX");
}

template<int cn>
class ShearY: public MatTransformmer<uchar, cn>
{
private:
    double degree;
    double tand;
public:
    ShearY(double degree):degree(degree){
        tand = tan(degree / 180 * 3.14159);
    }
    Point2f pointMap(Point2i point){
        return Point2f(point.x, point.y + point.x * tand);
    }
    Point2f pointMapReverse(Point2i point){
        return Point2f(point.x, point.y - point.x * tand);
    }
};

void MainWindow::on_buttonShearY_clicked()
{
    double degree;
    if (!ValueDialog().show("Degree", degree))
        return;

    Mat img;
    ui->image->getImage(img);

    if (img.channels() == 1)
        img = ShearY<1>(degree).doTrans(img);
    else if (img.channels() == 3)
        img = ShearY<3>(degree).doTrans(img);
    else
        return;

    ui->image->showImage(img);
    ui->history->addImg(img, "ShearY");
}

template<typename T, int channels>
class MatAdd: public MatOperator<T, channels>
{
    Vec<T, channels> op(Vec<T, channels> d1, Vec<T, channels> d2)
    {
        return d1 + d2;
    }
};

void MainWindow::on_buttonAdd_clicked()
{
    vector<string> imgList = ui->register_2->getImgNames();
    int idx;
    if (!ChoiceDialog().show("choose another image", imgList, idx))
        return;

    Mat img1;
    ui->image->getImage(img1);
    Mat img2 = ui->register_2->getImgById(idx);

    if (img1.rows != img2.rows || img1.cols != img2.cols) {
        MessageDialog().show("Size not equal");
        return;
    }

    Mat result;
    qDebug() << img1.channels() << "," << img2.channels() << endl;
    if (img1.channels() == 1 && img2.channels() == 1)
        result = MatAdd<uchar, 1>().doOp(img1, img2);
    else if (img1.channels() == 1) {
        img1.convertTo(img1, CV_8UC3);
        result = MatAdd<uchar, 3>().doOp(img1, img2);
    }
    else {
        img2.convertTo(img2, CV_8UC3);
        result = MatAdd<uchar, 3>().doOp(img1, img2);
    }

    ui->image->showImage(result);
    ui->history->addImg(result, "Add");
}

template<typename T, int channels>
class MatSub: public MatOperator<T, channels>
{
    Vec<T, channels> op(Vec<T, channels> d1, Vec<T, channels> d2)
    {
        return d1 - d2;
    }
};

void MainWindow::on_buttonSubstract_clicked()
{
    vector<string> imgList = ui->register_2->getImgNames();
    int idx;
    if (!ChoiceDialog().show("choose another image", imgList, idx))
        return;

    Mat img1;
    ui->image->getImage(img1);
    Mat img2 = ui->register_2->getImgById(idx);

    if (img1.rows != img2.rows || img1.cols != img2.cols) {
        MessageDialog().show("Size not equal");
        return;
    }

    Mat result;
    qDebug() << img1.channels() << "," << img2.channels() << endl;
    if (img1.channels() == 1 && img2.channels() == 1)
        result = MatSub<uchar, 1>().doOp(img1, img2);
    else if (img1.channels() == 1) {
        img1.convertTo(img1, CV_8UC3);
        result = MatSub<uchar, 3>().doOp(img1, img2);
    }
    else {
        img2.convertTo(img2, CV_8UC3);
        result = MatSub<uchar, 3>().doOp(img1, img2);
    }

    ui->image->showImage(result);
    ui->history->addImg(result, "Substract");
}

template<typename T, int cn>
class MatMul : public MatMapper<T, cn, T, cn>
{
private:
    double coef;
public:
    MatMul(double coef):coef(coef){};
    Vec<T, cn> map(Vec<T, cn> data) {
        return data * coef;
    }
};

void MainWindow::on_pushButton_2_clicked()
{
    double coef;
    if (!ValueDialog().show("coefficient", coef))
        return;

    Mat img;
    ui->image->getImage(img);

    if (img.channels() == 1)
        img = MatMul<uchar, 1>(coef).doMap(img);
    else
        img = MatMul<uchar, 3>(coef).doMap(img);

    ui->image->showImage(img);
    ui->history->addImg(img, "Multiply");
}

void MainWindow::on_buttonHistogram_clicked()
{
    Mat img;
    ui->image->getImage(img);

    int val;
    HistogramDialog().show(img, false, false, val);
}

void MainWindow::on_buttonEqualize_clicked()
{
    Mat img;
    ui->image->getImage(img);

    if (img.channels() == 1) {
        vector<int> histogram = getHistogram(img);
        vector<uchar> colormap = equalize(histogram);

        img = Adjuster(colormap).doMap(img);
    }
    else {
        Mat red = RGBToGray<uchar>(RED).doMap(img);
        Mat green = RGBToGray<uchar>(GREEN).doMap(img);
        Mat blue = RGBToGray<uchar>(BLUE).doMap(img);
        vector<int> h1 = getHistogram(img, RED);
        vector<int> h2 = getHistogram(img, GREEN);
        vector<int> h3 = getHistogram(img, BLUE);
        vector<uchar> c1 = equalize(h1);
        vector<uchar> c2 = equalize(h2);
        vector<uchar> c3 = equalize(h3);
        red = Adjuster(c1).doMap(red);
        green = Adjuster(c2).doMap(green);
        blue = Adjuster(c3).doMap(blue);
        red = GrayToRGB<uchar>(RED).doMap(red);
        green = GrayToRGB<uchar>(GREEN).doMap(green);
        blue = GrayToRGB<uchar>(BLUE).doMap(blue);
        img = MatAdd<uchar, 3>().doOp(MatAdd<uchar, 3>().doOp(red, green), blue);
    }
    ui->image->showImage(img);
    ui->history->addImg(img, QString::fromStdString("Equalize"));
}

void MainWindow::on_buttonGamma_clicked()
{
    double gamma;
    if (!ValueDialog().show("gamma", gamma))
        return;

    Mat img;
    ui->image->getImage(img);

    if (img.channels() == 1)
        img = Gamma<1>(gamma).doMap(img);
    else
        img = Gamma<3>(gamma).doMap(img);

    ui->image->showImage(img);
    ui->history->addImg(img, QString::fromStdString("Gamma"));
}

void MainWindow::on_pushButton_3_clicked()
{
    Mat img;
    ui->image->getImage(img);

    double hbias, sgamma, vgamma;
    if (!HSVDialog().show(hbias, sgamma, vgamma))
        return;

    img = RGBToHSVConverter().doMap(img);
    img = HSVAdjuster(hbias, sgamma, vgamma).doMap(img);
    img = HSVToRGBConverter().doMap(img);

    ui->image->showImage(img);
    ui->history->addImg(img, QString::fromStdString("HSV Adjust"));
}

void MainWindow::on_buttonThin_clicked()
{
    Mat img;
    ui->image->getImage(img);
    img = thin(img);
    ui->image->showImage(img);
    ui->history->addImg(img, QString::fromStdString("Thin"));
}

void MainWindow::on_buttonThick_clicked()
{
    Mat img;
    ui->image->getImage(img);
    img = thick(img);
    ui->image->showImage(img);
    ui->history->addImg(img, QString::fromStdString("Thick"));
}

void MainWindow::on_buttonDistance_clicked()
{
    Mat img;
    ui->image->getImage(img);

    img = distanceConvert(img);
    ui->image->showImage(img);
    ui->history->addImg(img, QString::fromStdString("Distance"));
}

void MainWindow::on_buttonBone_clicked()
{
    Mat img;
    ui->image->getImage(img);
    img = bone(img);
    ui->image->showImage(img);
    ui->history->addImg(img, QString::fromStdString("Bone"));
}

void MainWindow::on_buttonReconstruct_clicked()
{
    vector<string> imgList = ui->register_2->getImgNames();
    int idx;
    if (!ChoiceDialog().show("choose mark", imgList, idx))
        return;

    Mat img1;
    ui->image->getImage(img1);
    Mat img2 = ui->register_2->getImgById(idx);

    img1 = reconstruct(img1, img2);
    ui->image->showImage(img1);
    ui->history->addImg(img1, QString::fromStdString("Reconstruct"));
}

void MainWindow::on_buttonWatershed_clicked()
{
    double threshold;
    if (!ValueDialog().show("threshold", threshold, true))
        return;

    Mat img;
    ui->image->getImage(img);

    img = watershed(img, threshold);
    ui->image->showImage(img);
    ui->history->addImg(img, QString::fromStdString("Watershed"));
}
