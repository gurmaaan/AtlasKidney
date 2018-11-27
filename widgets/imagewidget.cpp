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
    createPreviewElemnts(imagesLocalPathes.size());
    QList<QObject*> previewElemnts = ui->carousel_h_layout->children();
    for(int i = 0 ; i < imagesLocalPathes.size(); i++)
    {
        QString fullPath = baseAbsolutePath + "/" + imagesLocalPathes.at(i);
        qDebug() << fullPath;
        if(fileExists(fullPath))
        {
            //QPixmap imgFromList(fullPath);
            images_.push_back(QPixmap(fullPath));
        }
        else
        {
            qDebug() << fullPath << "doesn't exist";
            images_.push_back(createPixmapWithtext(ui->first_label->size(), imagesLocalPathes.at(i)));
        }
    }
    ui->first_label->setPixmap(images_.at(0));
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
    for(int i = 0 ; i < cl->count(); i++)
        cl->removeItem(cl->itemAt(i));

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

bool ImageWidget::fileExists(QString path)
{
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

QPixmap ImageWidget::createPixmapWithtext(QSize size, QString text)
{
    QPixmap pixmap(size);
    pixmap.fill( QColor(PREVIEW_COLOR) );
    QPainter painter( &pixmap );
    painter.setFont( QFont("Arial") );
    painter.drawText( QPoint(size.width() / 10, size.height() / 10), text );
    return pixmap;
}

void ImageWidget::setImgNames(const QStringList &imgNames)
{
    imgNames_ = imgNames;
    loadImages(basePath_, imgNames_);
}

void ImageWidget::setBasePath(const QString &basePath)
{
    basePath_ = basePath;
}

void ImageWidget::setImages(const QVector<QPixmap> &value)
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
