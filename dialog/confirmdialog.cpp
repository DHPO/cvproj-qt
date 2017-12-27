#include "confirmdialog.h"
#include "ui_confirmdialog.h"

ConfirmDialog::ConfirmDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfirmDialog)
{
    ui->setupUi(this);
}

ConfirmDialog::~ConfirmDialog()
{
    delete ui;
}

bool ConfirmDialog::show(string message)
{
    ui->message->setText(QString::fromStdString(message));
    this->exec();
    return this->result();
}
