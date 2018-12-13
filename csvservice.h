#ifndef CSVSERVICE_H
#define CSVSERVICE_H

#include <QString>
#include <QPoint>
#include <QFile>
#include <QObject>
#include <QGraphicsItem>

enum class Sign
{
    Arrow,
    Ellipse,
    Square
};

enum class SignType
{
    Arrow,
    Square,
    Ellipse
};

class ImgGraphicsObject
{
public:
    ImgGraphicsObject();
    ImgGraphicsObject(QString imgName, int microFeatureNum, Sign type, QPoint startPoint, QPoint endPoint);
    QGraphicsItem *graphicsItem() const;

private:
    QString imgName_;
    int microFeatureNum_;
    Sign type_;
    QPoint startPoint_;
    QPoint endPoint_;
    QGraphicsItem *graphicsItem_;
    QGraphicsItem *genGraphicsItem();

};

class CSVService : public QObject
{
    Q_OBJECT
public:
    explicit CSVService(QObject *parent = nullptr);

    QVector<ImgGraphicsObject> readCSVFile(QString path) const;
    void setGraphicsObjects(const QVector<ImgGraphicsObject> &graphicsObjects);

signals:

public slots:

private:
    QVector<ImgGraphicsObject> graphicsObjects_;

};

#endif // CSVSERVICE_H
