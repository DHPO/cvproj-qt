#include "cvwidget.h"
#include <QDebug>
#include <QMouseEvent>
#include <sstream>

CVWidget::CVWidget(QWidget *parent) :
QOpenGLWidget(parent)
{
    mBgColor = QColor::fromRgb(150, 150, 150);
    scale = 1.0;
    offsetX = 0;
    offsetY = 0;
}

void CVWidget::initializeGL()
{
    makeCurrent();
    initializeOpenGLFunctions();

    float r = ((float)mBgColor.darker().red())/255.0f;
    float g = ((float)mBgColor.darker().green())/255.0f;
    float b = ((float)mBgColor.darker().blue())/255.0f;
    glClearColor(r,g,b,1.0f);
}

void CVWidget::resizeGL(int width, int height)
{
    makeCurrent();
    glViewport(0, 0, (GLint)width, (GLint)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0, width, -height, 0, 0, 1);

    glMatrixMode(GL_MODELVIEW);

    recalculatePosition();

    emit imageSizeChanged(mRenderWidth, mRenderHeight);

    updateScene();
}

void CVWidget::updateScene()
{
    if (this->isVisible()) update();
}

void CVWidget::paintGL()
{
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderImage();
}

void CVWidget::renderImage()
{

    drawMutex.lock();
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT);

    if (!mRenderQtImg.isNull())
    {
        glLoadIdentity();

        glPushMatrix();
        {
            /*if (mResizedImg.width() <= 0)
            {
                if (mRenderWidth == mRenderQtImg.width() && mRenderHeight == mRenderQtImg.height())
                    mResizedImg = mRenderQtImg;
                else
                    mResizedImg = mRenderQtImg.scaled(QSize(mRenderWidth, mRenderHeight),
                                                      Qt::IgnoreAspectRatio,
                                                      Qt::FastTransformation);
            }*/
            mResizedImg = mRenderQtImg.scaled(QSize(mRenderQtImg.width() * scale, mRenderQtImg.height() * scale),
                                              Qt::IgnoreAspectRatio,
                                              Qt::FastTransformation);

            // ---> Centering image in draw area
            mRenderPosX = (mResizedImg.width() - this->width()) * -offsetX;
            mRenderPosY = (mResizedImg.height() - this->height()) * offsetY;

//            glRasterPos2i(mRenderPosX, mRenderPosY);
            glRasterPos2i(0, 0);
//            glTranslatef(offsetX, offsetY, 0);
            glWindowPos2i(mRenderPosX, mRenderPosY + this->height());

            glPixelZoom(1, -1);

            glDrawPixels(mResizedImg.width(), mResizedImg.height(), GL_RGBA, GL_UNSIGNED_BYTE, mResizedImg.bits());

        }
        glPopMatrix();

        // end
        glFlush();
    }

    drawMutex.unlock();
}

void CVWidget::recalculatePosition()
{
    mImgRatio = (float)mOrigImage.cols/(float)mOrigImage.rows;

    mRenderWidth = this->size().width();
    mRenderHeight = floor(mRenderWidth / mImgRatio);

    if (mRenderHeight > this->size().height())
    {
        mRenderHeight = this->size().height();
        mRenderWidth = floor(mRenderHeight * mImgRatio);
    }

    mRenderPosX = floor((this->size().width() - mRenderWidth) / 2);
    mRenderPosY = -floor((this->size().height() - mRenderHeight) / 2);

    mResizedImg = QImage();
}

void CVWidget::showImage(Mat image)
{
    image.copyTo(origin);
    drawMutex.lock();
    if (image.depth() != 0)
        image.convertTo(image, CV_8UC(image.channels()));

    if (image.channels() == 3)
        cvtColor(image, mOrigImage, CV_BGR2RGBA);
    else if (image.channels() == 1)
        cvtColor(image, mOrigImage, CV_GRAY2RGBA);
    else if (image.channels() == 4)
        mOrigImage = image;
//    else return false;

    mRenderQtImg = QImage((const unsigned char*)(mOrigImage.data),
                          mOrigImage.cols, mOrigImage.rows,
                          mOrigImage.step1(), QImage::Format_RGB32);

    recalculatePosition();

    updateScene();
    drawMutex.unlock();
//    return true;
}

void CVWidget::mouseMoveEvent(QMouseEvent *e) {
    std::stringstream stream;
    int x = e->pos().x() - mRenderPosX;
    int y = e->pos().y() + mRenderPosY;
    if (x < mRenderWidth && y < mRenderHeight && x >=0 && y >= 0) {
        QRgb rgb = mResizedImg.pixel(x, y);
        stream << "(" << qRed(rgb) << "," << qGreen(rgb) << "," << qBlue(rgb) << ")";
        emit showColor(QString::fromStdString(stream.str()));
    }
}

void CVWidget::setScale(double scale) {
    this->scale = scale;
    renderImage();
}

void CVWidget::setOffsetX(int offsetX)
{
    this->offsetX = double(offsetX) / 100;
    renderImage();
}

void CVWidget::setOffsetY(int offsetY)
{
    this->offsetY = double(offsetY) / 100;
    renderImage();
}

void CVWidget::pushSave(QString name)
{
    emit save(mOrigImage, name);
}

void CVWidget::getImage(Mat &image)
{
    origin.copyTo(image);
    image.convertTo(image, CV_8UC(image.channels()));
}
