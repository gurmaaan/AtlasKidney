#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QFileInfo>
#include <QStandardItem>
#include <QGraphicsView>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <QDesktopWidget>
#include <static.h>

namespace Ui {
class ImageWidget;
}

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);
    ~ImageWidget();
    void loadImages(QString baseAbsolutePath, QStringList imagesLocalPathes);
    void setFrontImage(const QImage &value);
    void setImages(const QVector<QPixmap> &value);

public slots:
    void setBasePath(const QString &basePath);
    void setImgNames(const QStringList &imgNames);

private slots:
    void on_fullscreen_toolbtn_clicked();

private:
    Ui::ImageWidget *ui;
    QStandardItemModel *previewModel_;
    QImage frontImage_;
    QVector<QPixmap> images_;
    QString basePath_;
    QStringList imgNames_;

    bool fileExists(QString path);
    QPixmap createPixmapWithtext(QString text, QSize size = QSize(100, 100));
};

#endif // IMAGEWIDGET_H
