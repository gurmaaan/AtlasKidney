#include "csvservice.h"

CSVService::CSVService(QObject *parent) : QObject(parent)
{

}

QVector<ImgGraphicsObject> CSVService::readCSVFile(QString path) const
{
    QFile csvFIle(path);
    return graphicsObjects_;
}

void CSVService::setGraphicsObjects(const QVector<ImgGraphicsObject> &graphicsObjects)
{
    graphicsObjects_ = graphicsObjects;
}

ImgGraphicsObject::ImgGraphicsObject() {}

ImgGraphicsObject::ImgGraphicsObject(QString imgName, int microFeatureNum, Sign type, QPoint startPoint, QPoint endPoint)
    :imgName_(imgName), microFeatureNum_(microFeatureNum),type_(type), startPoint_(startPoint), endPoint_(endPoint)
{
    graphicsItem_ = genGraphicsItem();
}

QGraphicsItem *ImgGraphicsObject::graphicsItem() const
{
    return graphicsItem_;
}

QGraphicsItem *ImgGraphicsObject::genGraphicsItem()
{
}
