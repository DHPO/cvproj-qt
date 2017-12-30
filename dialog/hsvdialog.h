#ifndef HSVDIALOG_H
#define HSVDIALOG_H

#include <QDialog>

namespace Ui {
class HSVDialog;
}

class HSVDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HSVDialog(QWidget *parent = 0);
    ~HSVDialog();
    bool show(double &hbias, double &sgamma, double &vgamma);

private:
    Ui::HSVDialog *ui;
};

#endif // HSVDIALOG_H
