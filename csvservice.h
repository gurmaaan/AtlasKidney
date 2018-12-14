#ifndef CSVSERVICE_H
#define CSVSERVICE_H

#include <QString>
#include <QPoint>
#include <QFile>
#include <QObject>
#include <QGraphicsItem>
#include <QTextStream>
#include <QDebug>

enum class Sign
{
    Arrow,
    Ellipse,
    Square,
    NoSign
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
    QString imgName() const;
    int microFeatureNum() const;
    Sign type() const;
    QPoint startPoint() const;
    QPoint endPoint() const;

private:
    QString imgName_;
    int microFeatureNum_;
    Sign type_;
    QPoint startPoint_;
    QPoint endPoint_;
    QGraphicsItem *graphicsItem_;
    QGraphicsItem *genGraphicsItem();

};

QDebug operator<< (QDebug os, const ImgGraphicsObject& igo);

class CSVService : public QObject
{
    Q_OBJECT
public:
    explicit CSVService(QObject *parent = nullptr);

    QVector<ImgGraphicsObject> readCSVFile(QString path) const;
    void setGraphicsObjects(const QVector<ImgGraphicsObject> &graphicsObjects);
    static QString printSign(const Sign &s);

private:
    QVector<ImgGraphicsObject> graphicsObjects_;
    Sign detectSign(QString signStr) const;
    QPoint detectPoint(QString pointStr) const;

};

#endif // CSVSERVICE_H
