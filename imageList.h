#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class ImageList : public QListWidget
{
    Q_OBJECT
public:
    ImageList(QWidget *parent);

signals:
    void showImg(const Mat &img);

public slots:
    void addImg(Mat img, QString name);

private slots:
    void getImg(QListWidgetItem *item);

private:
    vector<Mat *> imgs;
};

#endif // IMAGELIST_H
