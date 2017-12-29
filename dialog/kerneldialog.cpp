#include "kerneldialog.h"
#include "ui_kerneldialog.h"
#include <QDebug>
#include "./filter/filter_basic.h"
#include "./messagedialog.h"

KernelDialog::KernelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KernelDialog)
{
    ui->setupUi(this);
    ui->kernel->resizeColumnsToContents();
    ui->kernel->resizeRowsToContents();
}

KernelDialog::~KernelDialog()
{
    delete ui;
}

void KernelDialog::on_boxRow_valueChanged(int arg1)
{
    ui->kernel->setRowCount(arg1);
    ui->kernel->resizeRowsToContents();
    ui->kernel->resizeColumnsToContents();
}

void KernelDialog::on_boxCol_valueChanged(int arg1)
{
    ui->kernel->setColumnCount(arg1);
    ui->kernel->resizeColumnsToContents();
    ui->kernel->resizeRowsToContents();
}

void KernelDialog::setDefault()
{
    int rows = ui->kernel->rowCount();
    int cols = ui->kernel->columnCount();

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (ui->kernel->item(r, c) == NULL) {
                ui->kernel->setItem(r, c, new QTableWidgetItem());
                ui->kernel->item(r, c)->setData(Qt::EditRole, QVariant(1.0));
            }
        }
    }
}

Mat KernelDialog::getKernel()
{
    int rows = ui->kernel->rowCount();
    int cols = ui->kernel->columnCount();

    Mat kernel(rows, cols, CV_32FC1);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            float data = ui->kernel->item(r, c)->data(Qt::EditRole).toFloat();
            kernel.at<float>(r, c) = data;
//            qDebug() << data << endl;
        }
    }

    return kernel;
}

void KernelDialog::setKernel(const Mat &kernel)
{
    int rows = ui->kernel->rowCount();
    int cols = ui->kernel->columnCount();

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            float data = kernel.at<float>(r, c);
            ui->kernel->item(r, c)->setData(Qt::EditRole, QVariant(data));
        }
    }
    ui->kernel->resizeColumnsToContents();
    ui->kernel->resizeRowsToContents();
}

void KernelDialog::on_pushButton_clicked()
{
    Mat kernel = getKernel();
    try {
        kernel = kernelUniform(kernel);
        setKernel(kernel);
    }
    catch(std::string s) {
        MessageDialog().show(s);
    }
}

bool KernelDialog::show(Mat &kernel) {
    exec();
    getKernel().copyTo(kernel);
    return this->result();
}

void KernelDialog::on_pushButton_2_clicked()
{
    setDefault();
}
