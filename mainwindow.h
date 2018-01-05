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
    void openFile();
    void saveas();

    void on_buttonOpen_clicked();

    void on_buttonSaveAs_clicked();

    void on_buttonCanay_clicked();

    void on_buttonSobel_clicked();

    void on_buttonToGrayscale_clicked();

    void on_buttonLaplace_clicked();

    void on_buttonGauss_clicked();

    void on_buttonMean_clicked();

    void on_buttonCustom_clicked();

    void on_buttonMedium_clicked();

    void on_buttonToBinary_clicked();

    void on_buttonDilate_clicked();

    void on_buttonErode_clicked();

    void on_buttonOpen_2_clicked();

    void on_buttonClose_clicked();

    void on_buttonGrad_clicked();

    void on_buttonTophat_clicked();

    void on_buttonBlackhat_clicked();

    void on_buttonScale_clicked();

    void on_buttonRotate_clicked();

    void on_buttonShearX_clicked();

    void on_buttonShearY_clicked();

    void on_buttonAdd_clicked();

    void on_buttonSubstract_clicked();

    void on_pushButton_2_clicked();

    void on_buttonHistogram_clicked();

    void on_buttonEqualize_clicked();

    void on_buttonGamma_clicked();

    void on_pushButton_3_clicked();

    void on_buttonThin_clicked();

    void on_buttonThick_clicked();

    void on_buttonDistance_clicked();

    void on_buttonBone_clicked();

    void on_buttonReconstruct_clicked();

    void on_buttonWatershed_clicked();

    void on_buttonCut_clicked();

    void on_buttonToRGB_clicked();

    void on_buttonLinear_clicked();

private:
    Ui::MainWindow *ui;
    QAction *newAct;
};

#endif // MAINWINDOW_H
