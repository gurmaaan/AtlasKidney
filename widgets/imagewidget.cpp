#include "imagewidget.h"
#include "ui_imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget)
{
    ui->setupUi(this);
}

ImageWidget::~ImageWidget()
{
    delete ui;
}

void ImageWidget::loadImages(QString baseAbsolutePath, QStringList imagesLocalPathes)
{
//    qDebug() << "Input list to load function"
    createPreviewElemnts(imagesLocalPathes.size());
}

void ImageWidget::createPreviewElemnts(int numOfElemnts)
{

}

void ImageWidget::setImages(const QVector<QImage> &value)
{
    images = value;
}

void ImageWidget::setFrontImage(const QImage &value)
{
    frontImage = value;
}
