#ifndef HISTOGRAMDIALOG_H
#define HISTOGRAMDIALOG_H

#include <QDialog>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

namespace Ui {
class HistogramDialog;
}

class HistogramDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HistogramDialog(QWidget *parent = 0);
    ~HistogramDialog();
    bool show(const Mat &img, bool showSlideBar, int &value);

private slots:
    void on_mode_currentIndexChanged(int index);

private:
    Ui::HistogramDialog *ui;
    void setGraph(vector<int> datas);
    vector<int> datas;
    Mat img;

};

#endif // HISTOGRAMDIALOG_H
