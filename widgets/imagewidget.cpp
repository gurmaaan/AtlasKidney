#include "imagewidget.h"
#include "ui_imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget)
{
    ui->setupUi(this);
    previewModel_ = new QStandardItemModel;
    ui->preview_table->setModel(previewModel_);
}

ImageWidget::~ImageWidget()
{
    delete ui;
}

void ImageWidget::loadImages(QString baseAbsolutePath, QStringList imagesLocalPathes)
{
    previewModel_->clear();
    QList<QStandardItem*> previewRow;
    for (int i = 0; i < imagesLocalPathes.size(); i++)
    {
        QString imgName = imagesLocalPathes.at(i).toLower();
        QString fullPath = baseAbsolutePath + "/" + imgName;
        QStandardItem* item = new QStandardItem();
        if(fileExists(fullPath))
        {
            item->setIcon(QIcon(fullPath));
        }
        else
        {            
            QString errorMsg = imagesLocalPathes.at(i) + "\n" + "doesn't exist";
            qDebug() << errorMsg << endl << fullPath << endl;
            QPixmap nullPixmap = createPixmapWithtext(errorMsg);
            item->setIcon(QIcon(nullPixmap));
        }
        previewRow.append(item);
    }
    previewModel_->appendRow(previewRow);
    ui->preview_table->setRowHeight(0,100);
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

QPixmap ImageWidget::createPixmapWithtext(QString text, QSize size)
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
    QDialog *fsDialog = new QDialog();
    QHBoxLayout *layout = ui->mainPhoto_H_layout;

//    QPixmap pixMap(imgPath);
//    label->setPixmap(pixMap);
//    layout->addWidget(label, 0, 0);
    fsDialog->setLayout(layout);
    fsDialog->showFullScreen();
}
