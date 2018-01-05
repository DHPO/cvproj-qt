#ifndef POINTSDIALOG_H
#define POINTSDIALOG_H

#include <QDialog>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

namespace Ui {
class PointsDialog;
}

class PointsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PointsDialog(QWidget *parent = 0);
    ~PointsDialog();
    bool show(bool colorMode, int maxPoint, vector<Point2i> &points);

private slots:
    void on_buttonAdd_clicked();

private:
    Ui::PointsDialog *ui;
    int maxPoint;
    vector<Point2i> points;
};

#endif // POINTSDIALOG_H
