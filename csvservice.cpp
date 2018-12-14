#include "csvservice.h"

CSVService::CSVService(QObject *parent) : QObject(parent)
{

}

QVector<ImgGraphicsObject> CSVService::readCSVFile(QString path) const
{
    QFile csvFIle(path);
    QVector<ImgGraphicsObject> grV;
    if( !csvFIle.open(QFile::ReadOnly | QFile::Text) )
    {
        qDebug() << "CSV file not exist";
    }
    else
    {
        QTextStream in(&csvFIle);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList strList = line.split('\t');
            if(strList.count() >=5)
                {
                QString imgName = strList.at(0);
                int featureNum = strList.at(1).toInt();
                Sign s = detectSign( strList.at(2));
                qDebug() << imgName << featureNum;
                printSign(s);
                QStringList fPStrL = strList.at(3).split(',');
                QPoint startP = QPoint(fPStrL.at(0).toInt(), fPStrL.at(1).toInt());
                QStringList sPStrL = strList.at(4).split(',');
                QPoint endP = QPoint(sPStrL.at(0).toInt(), sPStrL.at(1).toInt());
                qDebug() << startP << endP;
            }
        }
    }
    return graphicsObjects_;
}

void CSVService::setGraphicsObjects(const QVector<ImgGraphicsObject> &graphicsObjects)
{
    graphicsObjects_ = graphicsObjects;
}

Sign CSVService::detectSign(QString signStr) const
{
    Sign s;
    if(signStr == "\"Arrow\"")
        s = Sign::Arrow;
    else if(signStr == "\"Ellipse\"")
        s = Sign::Ellipse;
    else if(signStr == "\"Square\"")
        s = Sign::Square;
    else
        s = Sign::NoSign;
    return s;
}

void CSVService::printSign(const Sign &s)
{
    switch (s) {
    case Sign::Arrow:
        qDebug() << "Arrow";
        break;
    case Sign::Ellipse:
        qDebug() << "Ellipse";
        break;
    case Sign::Square:
        qDebug() << "Square";
        break;
    default:
        qDebug() << "No sign";
        break;
    }
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
