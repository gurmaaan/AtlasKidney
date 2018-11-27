#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QFileInfo>
#include <QPixmap>
#include <QLabel>
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
    void createPreviewElemnts(int numOfElemnts);
    QImage frontImage_;
    QVector<QPixmap> images_;
    QString basePath_;
    QStringList imgNames_;
    bool fileExists(QString path);
    QPixmap createPixmapWithtext(QSize size, QString text);
};

#endif // IMAGEWIDGET_H
