#include "imagewidget.h"
#include "ui_imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget)
{
    ui->setupUi(this);
    previewModel_ = new QStandardItemModel;
    scene_ = new QGraphicsScene;

    ui->preview_table->setModel(previewModel_);
    ui->main_photo_gv->setScene(scene_);
}

ImageWidget::~ImageWidget()
{
    delete ui;
}

void ImageWidget::loadImages(QString baseAbsolutePath, QStringList imagesLocalPathes)
{
    previewModel_->clear();
    scene_->clear();
    QList<QStandardItem*> previewRow;
    QVector<QPixmap> imgVector;
    for (int i = 0; i < imagesLocalPathes.size(); i++)
    {
        QString fullPath = baseAbsolutePath + "/" + imagesLocalPathes.at(i);
        QPixmap pixMapAtI(fullPath);
        //QImage imgAtI(fullPath);
        imgVector.push_back(pixMapAtI);
        QStandardItem* item = new QStandardItem();
        if(fileExists(fullPath))
        {
            item->setIcon(pixMapAtI);
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

    setImages(imgVector);
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(imgVector.at(0));
    scene_->addItem(item);
    ui->main_photo_gv->show();

    previewModel_->appendRow(previewRow);
    ui->preview_table->setRowHeight(0,100);
    ui->preview_table->setCurrentIndex( ui->preview_table->model()->index(0,0));
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

void ImageWidget::setFrontImage(const QPixmap &value)
{
    frontImage_ = value;

    scene_->clear();
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(value);
    scene_->addItem(item);
    ui->main_photo_gv->show();
}

void ImageWidget::on_fullscreen_toolbtn_clicked()
{
    QDialog *fsDialog = new QDialog;
    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *label = new QLabel;
    QPixmap activePm = images_.at(ui->preview_table->selectionModel()->selectedColumns(0).at(0).column());
    label->setPixmap(activePm.scaled( QApplication::desktop()->screenGeometry().size()));
    layout->addWidget(label);
    fsDialog->setLayout(layout);
    fsDialog->showFullScreen();
}

void ImageWidget::on_preview_table_clicked(const QModelIndex &index)
{
    setFrontImage(images_.at(index.column()));
}
