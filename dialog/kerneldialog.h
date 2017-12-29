#ifndef KERNELDIALOG_H
#define KERNELDIALOG_H

#include <QDialog>
#include <opencv2/opencv.hpp>
using namespace cv;

namespace Ui {
class KernelDialog;
}

class KernelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KernelDialog(QWidget *parent = 0);
    ~KernelDialog();
    bool show(Mat &kernel);

private slots:
    void on_boxRow_valueChanged(int arg1);

    void on_boxCol_valueChanged(int arg1);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::KernelDialog *ui;
    Mat getKernel();
    void setKernel(const Mat &kernel);
    void setDefault();
};

#endif // KERNELDIALOG_H
