#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include <QDialog>
#include <string>
using namespace std;

namespace Ui {
class ConfirmDialog;
}

class ConfirmDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDialog(QWidget *parent = 0);
    ~ConfirmDialog();
    bool show(string message);

private:
    Ui::ConfirmDialog *ui;
};

#endif // CONFIRMDIALOG_H
