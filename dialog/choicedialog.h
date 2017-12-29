#ifndef CHOICEDIALOG_H
#define CHOICEDIALOG_H

#include <QDialog>
#include <string>
#include <vector>
using namespace std;

namespace Ui {
class ChoiceDialog;
}

class ChoiceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChoiceDialog(QWidget *parent = 0);
    ~ChoiceDialog();
    bool show(string name, vector<string> items, int &choice);

private:
    Ui::ChoiceDialog *ui;
};

#endif // CHOICEDIALOG_H
