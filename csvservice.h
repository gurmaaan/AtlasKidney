#ifndef CSVSERVICE_H
#define CSVSERVICE_H

#include <QString>
#include <QPoint>
#include <QObject>
#include <QGraphicsEllipseItem>

class ImgGraphicsObject
{
public:
    ImgGraphicsObject() {}
private:
    QString imgName;
    QPoint startPoint;
    QPoint endPoint;
};

class CSVService : public QObject
{
    Q_OBJECT
public:
    explicit CSVService(QObject *parent = nullptr);

signals:

public slots:

private:

};

#endif // CSVSERVICE_H
