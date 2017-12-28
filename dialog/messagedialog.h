#ifndef MESSAGEDIALOG_H
#define MESSAGEDIALOG_H

#include <QDialog>
#include <string>
using namespace std;

namespace Ui {
class MessageDialog;
}

class MessageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessageDialog(QWidget *parent = 0);
    ~MessageDialog();
    void show(string message);

private:
    Ui::MessageDialog *ui;
};

#endif // MESSAGEDIALOG_H
