#include "hsvdialog.h"
#include "ui_hsvdialog.h"

HSVDialog::HSVDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HSVDialog)
{
    ui->setupUi(this);
}

HSVDialog::~HSVDialog()
{
    delete ui;
}

bool HSVDialog::show(double &hbias, double &sgamma, double &vgamma)
{
    exec();
    hbias = ui->hbias->value();
    sgamma = ui->sgamma->value();
    vgamma = ui->vgamma->value();
    return this->result();
}
