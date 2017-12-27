#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void open();
    void saveas();

    void on_buttonOpen_clicked();

    void on_buttonSaveAs_clicked();

    void on_buttonCanay_clicked();

    void on_buttonSobel_clicked();

    void on_buttonToGrayscale_clicked();

private:
    Ui::MainWindow *ui;
    QAction *newAct;
};

#endif // MAINWINDOW_H
