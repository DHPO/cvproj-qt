#include "choicedialog.h"
#include "ui_choicedialog.h"

ChoiceDialog::ChoiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoiceDialog)
{
    ui->setupUi(this);
}

ChoiceDialog::~ChoiceDialog()
{
    delete ui;
}

bool ChoiceDialog::show(string name, vector<string> items, int &choice)
{
    ui->name->setText(QString::fromStdString(name));
    for (int i = 0; i < items.size(); i++) {
        ui->choice->insertItem(i, QString::fromStdString(items[i]));
    }
    exec();
    choice = ui->choice->currentIndex();
    return this->result();
}
