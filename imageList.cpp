#include "imageList.h"
#include <QDebug>

ImageList::ImageList(QWidget *parent):QListWidget(parent)
{
    connect(this, &ImageList::itemClicked, this, &ImageList::getImg);
}

void ImageList::getImg(QListWidgetItem *item)
{
    Mat *img = imgs[item->text()];
    emit showImg(*img);
}

void ImageList::addImg(Mat img, QString name)
{
    Mat *pimg = new Mat(img);
    while(imgs.find(name) != imgs.end()) {
        name.append("~");
    }
    imgs[name] = pimg;
    addItem(name);
}
