#ifndef VALUEDIALOG_H
#define VALUEDIALOG_H

#include <QDialog>
#include <string>
using namespace std;

namespace Ui {
class ValueDialog;
}

class ValueDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ValueDialog(QWidget *parent = 0);
    ~ValueDialog();
    bool show(string name, double &value, bool integer = false, double max = 10000, double min = -10000);

private:
    Ui::ValueDialog *ui;
};

#endif // VALUEDIALOG_H
