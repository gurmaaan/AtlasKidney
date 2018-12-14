#include "csvservice.h"

CSVService::CSVService(QObject *parent) : QObject(parent)
{

}

QVector<ImgGraphicsObject> CSVService::readCSVFile(QString path) const
{
    QFile csvFIle(path);
    QVector<ImgGraphicsObject> iGOV;
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
                QPoint startPoint = detectPoint( strList.at(3) );
                QPoint endPoint = detectPoint( strList.at(4) );
                ImgGraphicsObject imgGrO(imgName, featureNum, s, startPoint, endPoint);
                qDebug() << imgGrO;
                iGOV.push_back(imgGrO);
            }
            else
                continue;
        }
    }
    return iGOV;
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

QString CSVService::printSign(const Sign &s)
{
    QString signStr;
    switch (s) {
    case Sign::Arrow:
        signStr = "Arrow";
        break;
    case Sign::Ellipse:
        signStr = "Ellipse";
        break;
    case Sign::Square:
        signStr = "Square";
        break;
    default:
        signStr = "No sign";
        break;
    }
    return signStr;
}

QPoint CSVService::detectPoint(QString pointStr) const
{
    QPoint p;
    QStringList strList = pointStr.split(',');
    if(strList.count() >= 2)
    {
        p = QPoint(strList.at(0).toInt(), strList.at(1).toInt());
    }
    else
    {
        p = QPoint(0,0);
    }
    return p;
}

ImgGraphicsObject::ImgGraphicsObject() {}

ImgGraphicsObject::ImgGraphicsObject(QString imgName, int microFeatureNum, Sign type, QPoint startPoint, QPoint endPoint)
    :imgName_(imgName), microFeatureNum_(microFeatureNum),type_(type), startPoint_(startPoint), endPoint_(endPoint)
{
    //graphicsItem_ = genGraphicsItem();
}

QGraphicsItem *ImgGraphicsObject::graphicsItem() const
{
    return graphicsItem_;
}

QString ImgGraphicsObject::imgName() const
{
    return imgName_;
}

int ImgGraphicsObject::microFeatureNum() const
{
    return microFeatureNum_;
}

Sign ImgGraphicsObject::type() const
{
    return type_;
}

QPoint ImgGraphicsObject::startPoint() const
{
    return startPoint_;
}

QPoint ImgGraphicsObject::endPoint() const
{
    return endPoint_;
}


QGraphicsItem *ImgGraphicsObject::genGraphicsItem()
{
}

QDebug operator<<(QDebug os, const ImgGraphicsObject &igo)
{
    os << igo.imgName() << endl <<
      "\t"   << igo.microFeatureNum() << endl <<
      "\t\t" << CSVService::printSign(igo.type()) << endl <<
      "\t\t" << igo.startPoint() << endl <<
      "\t\t" << igo.endPoint() << endl;
    return os;
}
