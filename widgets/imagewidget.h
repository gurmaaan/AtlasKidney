#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QFileInfo>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <QDesktopWidget>
#include <QScreen>
#include <static.h>
#include <model/graphicsobject.h>
#include <databaseservice.h>

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
    void setImages(const QVector<QPixmap> &value);
    void scaleImage(int k);

    int frontImgIndex() const;
    void setFrontImgIndex(int frontImgIndex);

    PatientInfo patientInfo() const;
    void setPatientInfo(const PatientInfo &pi);

public slots:
    void setBasePath(const QString &basePath);
    void on_plus_toolbtn_clicked();
    void on_minus_toolbtn_clicked();
    void on_fullscreen_toolbtn_clicked();
    void setFrontImage(int findex);
    void setVisible(QString imgName, int maskID, bool state);

private slots:
    void on_preview_table_clicked(const QModelIndex &index);
    void on_zoom_v_slider_sliderMoved(int position);

signals:
    void frontImgIndexChenged(int findex);
    void selectedIndexChanged(int index);

private:
    Ui::ImageWidget *ui;
    QStandardItemModel *previewModel_;
    QPixmap frontImage_;
    PatientInfo pi_;
    QVector<QPixmap> images_;
    QString basePath_;
    QStringList imgNames_;
    QGraphicsScene *scene_;
    int k_;

    int frontImgIndex_;
    bool fileExists(QString path);
    QSize scaledSize(int k);
    void setWhatsStatus(QStandardItem *item, QString text);
    QPixmap createPixmapWithtext(QString text, QSize size = QSize(100, 100));
};

#endif // IMAGEWIDGET_H
