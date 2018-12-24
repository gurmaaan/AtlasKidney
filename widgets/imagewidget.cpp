#include "imagewidget.h"
#include "ui_imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget)
{
    ui->setupUi(this);
    previewModel_ = new QStandardItemModel;
    scene_ = new QGraphicsScene;
    k_ = 0;

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

    QList<QStandardItem*> previewRow;
    QList<QStandardItem*> imgNameRow;
    QVector<QPixmap> imgVector;

    for (int i = 0; i < imagesLocalPathes.size(); i++)
    {
        QString imgName = imagesLocalPathes.at(i);
        imgNames_.push_back(imgName);
        QString fullPath = baseAbsolutePath + "/" + imgName;
        QPixmap pixMapAtI(fullPath);
        imgVector.push_back(pixMapAtI);
        QStandardItem* item = new QStandardItem();
        if(fileExists(fullPath))
        {
            item->setIcon(pixMapAtI);
            setWhatsStatus(item, imgName);
        }
        else
        {            
            QPixmap nullPixmap = createPixmapWithtext(imgName);
            item->setIcon(QIcon(nullPixmap));
        }
        previewRow.append(item);

        QStandardItem* nameItem = new QStandardItem("Изображение " + QString::number(i+1));
        setWhatsStatus(nameItem, imgName);
        imgNameRow.append(nameItem);
    }

    setImages(imgVector);
    setFrontImgIndex(0);

    previewModel_->appendRow(previewRow);
    previewModel_->appendRow(imgNameRow);
    ui->preview_table->setRowHeight(0,100);
    ui->preview_table->selectColumn(0);
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

QSize ImageWidget::scaledSize(int k)
{
    double H0 = static_cast<double>(frontImage_.height());
    double W0 = static_cast<double>(frontImage_.width());
    double dk = static_cast<double>(k) / static_cast<double>(100);

    int W = static_cast<int>(W0 + W0*dk);
    int H = static_cast<int>(H0 + H0*dk);

    return QSize(W, H);
}

void ImageWidget::setWhatsStatus(QStandardItem *item, QString text)
{
    item->setWhatsThis(text);
    item->setStatusTip(text);
    item->setTextAlignment(Qt::AlignCenter);
}

QPixmap ImageWidget::createPixmapWithtext(QString text, QSize size)
{
    QPixmap pixmap(size);
    QString text2 = "doesn't exist";
    pixmap.fill( QColor(PREVIEW_COLOR) );
    QPainter painter( &pixmap );
    painter.setFont( QFont("Arial", 7) );
    QFontMetrics fm = painter.fontMetrics();
    int x1 = (size.width() - fm.boundingRect(text).width()) / 2;
    int y1 = (size.height() - (fm.height() * 2.5)) / 2;
    int x2 = (size.width() - fm.boundingRect(text2).width()) / 2;
    int y2 = y1 + fm.height()*1.5;
    painter.drawText(QPoint(x1, y1), text);
    painter.drawText(QPoint(x2, y2), text2);
    return pixmap;
}

void ImageWidget::scaleImage(int k)
{
    scene_->clear();
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(frontImage_.scaled(scaledSize(k), Qt::KeepAspectRatio));
    scene_->addItem(item);
    if(k ==0)
        ui->main_photo_gv->fitInView(item, Qt::KeepAspectRatio);
    else
        item->setPos(0, 0);
}

void ImageWidget::setBasePath(const QString &basePath)
{
    basePath_ = basePath;
}

void ImageWidget::setImages(const QVector<QPixmap> &value)
{
    images_ = value;
}

void ImageWidget::setFrontImage(int findex)
{
    QPixmap fImage = images_.at(findex);
    frontImage_ = fImage;
    scene_->clear();
    QGraphicsPixmapItem *pmI = new QGraphicsPixmapItem(frontImage_.scaled(scaledSize(0), Qt::KeepAspectRatio));
    scene_->addItem(pmI);
}

void ImageWidget::setVisible(QString imgName, int maskID, bool state)
{
    scene_->clear();

    QMap<QString, QMap<int, Feature>> images = pi_.getImages();
    int activeIndex = 0;
    for(int i = 0; i < images.keys().count(); i++)
    {
        if(images.keys().at(i) == imgName)
        {
            activeIndex = i;
            break;
        }

    }

    frontImage_ = images_.at(activeIndex);
    QGraphicsPixmapItem *pmI = new QGraphicsPixmapItem(frontImage_.scaled(scaledSize(0), Qt::KeepAspectRatio));
    scene_->addItem(pmI);

    if(state)
    {
        QVector<GraphicsObject> grObjcts = images[imgName][maskID].objs();
        for(GraphicsObject g : grObjcts)
        {
            QGraphicsItemGroup *group = g.genGrIGr(g.startPoint(), g.endPoint());
            scene_->addItem(group);
        }
    }
}

void ImageWidget::on_fullscreen_toolbtn_clicked()
{
    QDialog *fsDialog = new QDialog;
    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *label = new QLabel;

    QPixmap activePm;
    int index = ui->preview_table->selectionModel()->selectedIndexes().at(0).column();
    qDebug() << "Selected column " << index;
    if(index >= 0)
        activePm = images_.at(index);
    else
        activePm = images_.at(0);

    label->setPixmap(activePm.scaled( QGuiApplication::screens().at(0)->availableSize() ) );
    layout->addWidget(label);
    fsDialog->setLayout(layout);
    fsDialog->showFullScreen();
}

void ImageWidget::on_preview_table_clicked(const QModelIndex &index)
{
    setFrontImage(index.column());
    emit selectedIndexChanged(index.column());
}

void ImageWidget::on_plus_toolbtn_clicked()
{
    k_+=5;
    scaleImage(k_);
    ui->zoom_v_slider->setValue(k_);
}

void ImageWidget::on_minus_toolbtn_clicked()
{
    k_-=5;
    scaleImage(k_);
    ui->zoom_v_slider->setValue(k_);
}

void ImageWidget::on_zoom_v_slider_sliderMoved(int position)
{
    k_ = position;
    scaleImage(k_);
}

PatientInfo ImageWidget::patientInfo() const
{
    return pi_;
}

int ImageWidget::frontImgIndex() const
{
    return frontImgIndex_;
}

void ImageWidget::setFrontImgIndex(int frontImgIndex)
{
    frontImgIndex_ = frontImgIndex;
    emit frontImgIndexChenged(frontImgIndex);
}

void ImageWidget::setPatientInfo(const PatientInfo &pi)
{
    pi_ = pi;
    imgNames_ = pi.getImages().keys();
    loadImages(basePath_, imgNames_);
}
