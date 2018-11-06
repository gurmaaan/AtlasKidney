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
    QLayout* cl = ui->carousel_h_layout;
    QLabel* fl = ui->first_label;

    QString labelsStyleStr = fl->styleSheet();
    QRect labelsGeometry = fl->geometry();

    QRect labelsRect = fl->frameGeometry();
    int labelsStyle = fl->frameStyle();
    QFrame::Shape labelsShape = fl->frameShape();
    QFrame::Shadow labelsShadow = fl->frameShadow();

    QRect spacerSize = ui->carousel_h_spacer->geometry();
    QSpacerItem *cSpacer = new QSpacerItem(spacerSize.width() - (labelsGeometry.width() * numOfElemnts), spacerSize.height());
    cl->removeItem(cl->itemAt(1));

    for(int i = 0; i < numOfElemnts - 1; i++)
    {
        QLabel *imgLabel = new QLabel();
        imgLabel->setStyleSheet(labelsStyleStr);

        imgLabel->setFrameRect(labelsRect);
        imgLabel->setFrameShadow(labelsShadow);
        imgLabel->setFrameShape(labelsShape);
        imgLabel->setFrameStyle(labelsStyle);

        imgLabel->setGeometry(labelsGeometry);
        imgLabel->setMinimumWidth(fl->minimumWidth());
        imgLabel->setMinimumHeight(fl->minimumHeight());
        cl->addWidget(imgLabel);
    }

    cl->addItem(cSpacer);
}

void ImageWidget::setImages(const QVector<QImage> &value)
{
    images_ = value;
}

void ImageWidget::setFrontImage(const QImage &value)
{
    frontImage_ = value;
}

void ImageWidget::on_fullscreen_toolbtn_clicked()
{
    createPreviewElemnts(5);
}
