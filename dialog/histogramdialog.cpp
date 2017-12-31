#include "histogramdialog.h"
#include "ui_histogramdialog.h"
#include "./color/color_histogram.h"
#include "./color/color_colorspace.h"
#include <QChart>
#include <QBarSet>
#include <QBarSeries>
#include <QChartView>
using namespace QtCharts;

HistogramDialog::HistogramDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistogramDialog)
{
    ui->setupUi(this);

}

HistogramDialog::~HistogramDialog()
{
    delete ui;
}

void HistogramDialog::setGraph(vector<int> datas)
{
    this->datas = datas;
    QBarSet *set = new QBarSet(QString::fromStdString("GrayScale"));
    for (int i = 0; i < 256; i++) {
        *set << datas[i];
    }
    QBarSeries *series = new QBarSeries();
    series->append(set);
    series->setBarWidth(1);
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    ui->graphicsView->setChart(chart);
}

bool HistogramDialog::show(const Mat &img, bool showSlideBar, bool ignoreZero, int &value)
{
    this->img = img;
    if(!showSlideBar) {
        ui->label->hide();
        ui->horizontalSlider->hide();
        ui->buttonOtsu->hide();
    }
    if(img.channels() == 1) {
        ui->mode->hide();
        vector<int> datas = getHistogram(img);
        if (ignoreZero)
            datas[0] = 0;
        setGraph(datas);
    }
    else {
        vector<int> datas = getHistogram(img, RED);
        setGraph(datas);
    }

    exec();
    value = ui->horizontalSlider->value();
    return this->result();
}

void HistogramDialog::on_mode_currentIndexChanged(int index)
{
    ConvertMethod methods[] = {RED, GREEN, BLUE, AVG};
    vector<int> datas = getHistogram(this->img, methods[index]);
    setGraph(datas);
}

void HistogramDialog::on_buttonOtsu_clicked()
{
    int t = otsu(datas);
    ui->horizontalSlider->setValue(t);
}
