#include "valuedialog.h"
#include "ui_valuedialog.h"

ValueDialog::ValueDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ValueDialog)
{
    ui->setupUi(this);
}

ValueDialog::~ValueDialog()
{
    delete ui;
}

bool ValueDialog::show(string name, double &value, bool integer, double max, double min)
{
    ui->name->setText(QString::fromStdString(name));
    ui->value->setDecimals(integer?0:2);
    ui->value->setMaximum(max);
    ui->value->setMinimum(min);
    this->exec();
    if (this->result()) {
        value = ui->value->value();
        return true;
    }
    return false;
}
