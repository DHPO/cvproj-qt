#ifndef CVWIDGET_H
#define CVWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_2_0>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <mutex>
using namespace cv;

class CVWidget : public QOpenGLWidget, protected QOpenGLFunctions_2_0
{
    Q_OBJECT
public:
    explicit CVWidget(QWidget *parent = 0);
    void getImage(Mat &image);

signals:
    void    imageSizeChanged( int outW, int outH ); /// Used to resize the image outside the widget
    void    showColor(QString color);
    void	save(Mat img, QString name);

public slots:
    void    showImage(Mat image); /// Used to set the image to be viewed
    void	setScale(double scale);
    void	setOffsetX(int offsetX);
    void	setOffsetY(int offsetY);
    void	pushSave(QString name);

protected:
    void 	initializeGL(); /// OpenGL initialization
    void 	paintGL(); /// OpenGL Rendering
    void 	resizeGL(int width, int height);        /// Widget Resize Event

    void        updateScene();
    void        renderImage();
    void        mouseMoveEvent(QMouseEvent *e);

private:

    QImage      mRenderQtImg;           /// Qt image to be rendered
    QImage      mResizedImg;
    cv::Mat     mOrigImage;             /// original OpenCV image to be shown
    cv::Mat		origin;

    QColor      mBgColor;		/// Background color

    float       mImgRatio;             /// height/width ratio

    int mRenderWidth;
    int mRenderHeight;
    int mRenderPosX;
    int mRenderPosY;

    double scale;
    double offsetX;
    double offsetY;

    void recalculatePosition();

    std::mutex drawMutex;
};
#endif // CVWIDGET_H
