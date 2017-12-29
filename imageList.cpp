#include "imageList.h"
#include <QDebug>

ImageList::ImageList(QWidget *parent):QListWidget(parent)
{
    connect(this, &ImageList::itemClicked, this, &ImageList::getImg);
}

void ImageList::getImg(QListWidgetItem *item)
{
    QModelIndex index = indexFromItem(item);
    Mat img = imgs[index.row()];
    emit showImg(img);
}

void ImageList::addImg(Mat img, QString name)
{
    //Mat *pimg = new Mat(img);
    /*while(imgs.find(name) != imgs.end()) {
        name.append("~");
    }
    imgs[name] = pimg;*/
    addItem(name);
    names.push_back(name.toStdString());
    imgs.push_back(img);
}

Mat ImageList::getImgById(int idx)
{
    return imgs[idx];
}

vector<string> ImageList::getImgNames()
{
    return names;
}
