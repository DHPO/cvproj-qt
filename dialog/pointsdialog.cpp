#include "pointsdialog.h"
#include "ui_pointsdialog.h"
#include <string>
using namespace std;

PointsDialog::PointsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PointsDialog)
{
    ui->setupUi(this);
}

PointsDialog::~PointsDialog()
{
    delete ui;
}

void PointsDialog::on_buttonAdd_clicked()
{
    int x = ui->boxX->value();
    int y = ui->boxY->value();
    stringstream point;
    point.clear();
    point << "(" << x << ", " << y << ")";

    ui->points->addItem(QString::fromStdString(point.str()));
    this->points.push_back(Point2i(x, y));
    ui->boxX->setMinimum(x);
    ui->boxY->setMinimum(y);

    if (this->points.size() == maxPoint)
        ui->buttonAdd->setDisabled(true);
}

bool PointsDialog::show(bool colorMode, int maxPoint, vector<Point2i> &points)
{
    if (colorMode) {
        ui->boxX->setMaximum(255);
        ui->boxY->setMaximum(255);
    }
    this->maxPoint = maxPoint;
    exec();
    if (this->result()) {
        points = this->points;
        return true;
    }
    return false;
}
