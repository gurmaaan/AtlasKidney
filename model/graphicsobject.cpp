#include "graphicsobject.h"

GraphicsObject::GraphicsObject(QString imgName, int microFeatureID, Sign type, QPointF startPoint, QPointF endPoint) :
    imgName_(imgName), microFeatureID_(microFeatureID), type_(type), startPoint_(startPoint), endPoint_(endPoint)
{
    setGroup( genGrIGr(startPoint, endPoint) );
}

GraphicsObject::GraphicsObject(QStringList csvRow)
{
    if(csvRow.size() >=5)
    {
        setImgName(csvRow.at(0));
        setMicroFeatureID(csvRow.at(1));
        setType(csvRow.at(2));
        setStartPoint(csvRow.at(3));
        setEndPoint(csvRow.at(4));
        genGrIGr(startPoint_,endPoint_);
    }
}

GraphicsObject::GraphicsObject(const GraphicsObject &g)
{
    GraphicsObject(g.imgName(), g.microFeatureID(), g.type(), g.startPoint(), g.endPoint());
}

QString GraphicsObject::imgName() const
{
    return imgName_;
}

void GraphicsObject::setImgName(const QString &imgName)
{
    imgName_ = imgName;
}

int GraphicsObject::microFeatureID() const
{
    return microFeatureID_;
}

void GraphicsObject::setMicroFeatureID(int microFeatureID)
{
    microFeatureID_ = microFeatureID;
}

void GraphicsObject::setMicroFeatureID(const QString &mfStr)
{
    bool status = false;
    int mid = mfStr.toInt(&status);
    if(status)
        setMicroFeatureID(mid);
    else
        qDebug() << "Invalid micro feature conversion from string " << mfStr;
}

Sign GraphicsObject::type() const
{
    return type_;
}

void GraphicsObject::setType(const Sign &type)
{
    type_ = type;
}

void GraphicsObject::setType(const QString &typeStr)
{
    Sign s = detectSign(typeStr);
    setType(s);
}

QPointF GraphicsObject::startPoint() const
{
    return startPoint_;
}

void GraphicsObject::setStartPoint(const QPointF &startPoint)
{
    startPoint_ = startPoint;
}

void GraphicsObject::setStartPoint(const QString &spStr)
{
    QPointF sp = detectPoint(spStr);
    setStartPoint(sp);
}

QPointF GraphicsObject::endPoint() const
{
    return endPoint_;
}

void GraphicsObject::setEndPoint(const QPointF &endPoint)
{
    endPoint_ = endPoint;
}

void GraphicsObject::setEndPoint(const QString &epStr)
{
    QPointF ep = detectPoint(epStr);
    setEndPoint(ep);
}

QString GraphicsObject::printSign(const Sign &s)
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

QGraphicsItemGroup *GraphicsObject::group() const
{
    return group_;
}

void GraphicsObject::setGroup(QGraphicsItemGroup *group)
{
    group_ = group;
}

void GraphicsObject::calcGraphicsObject()
{
    setGroup( genGrIGr( startPoint(), endPoint() ) );
}

QGraphicsItemGroup *GraphicsObject::genGrIGr(QPointF tlp, QPointF brp)
{
    QGraphicsItemGroup *gr = new QGraphicsItemGroup;

    QRectF rect(tlp, brp);
    switch (this->type())
    {
        case Sign::Arrow:
        {
            qDebug() << tlp << brp;
            gr = genArrow(tlp, brp, 0.1);
        }
            break;
        case Sign::Ellipse:
        {
            QGraphicsEllipseItem *ellipseItem = new QGraphicsEllipseItem(rect);
            gr->addToGroup(ellipseItem);
        }
            break;
        case Sign::Square:
        {
            QGraphicsRectItem *rectItem = new QGraphicsRectItem(rect);
            gr->addToGroup(rectItem);
        }
            break;
        case Sign::NoSign:
        {
            QGraphicsTextItem *textItem = new QGraphicsTextItem("NULL");
            gr->addToGroup(textItem);
            break;
        }
    }
    setGroup(gr);
    return gr;
}

QGraphicsItemGroup *GraphicsObject::genArrow(QPointF ap, QPointF bp, qreal angleSizeK)
{
    QGraphicsItemGroup *arrowGroup = new QGraphicsItemGroup;
    QPen pen = QPen(QColor(Qt::red), 1, Qt::SolidLine, Qt::RoundCap);

    qreal xa = ap.rx();
    qreal xb = bp.rx();
    qreal ya = ap.ry();
    qreal yb = bp.ry();

    //TODO во всех формулах сделать исключения на нули
    qreal n = yb - ya;
    qreal m = xb - xa;

    qreal len = sqrt( pow( n, 2) + pow(m, 2) );

    qreal t,p;
    if(len != 0)
    {
        t = n / len;
        p = m / len;
    }
    else
    {
        t = 0;
        p = 0;
    }

    qreal h = len * angleSizeK;

    qreal xc = xb - (p*h)*( (m-n)/(sqrt(3) * m) );
    qreal yc = yb - (t*h)*( (sqrt(3)*n + m)/(sqrt(3) * n) );

    qreal xd = xb + (p*h)*( (m*(1 - 2*sqrt(3)) - n)/(sqrt(3) * m) );
    qreal yd = yb + (t*h)*( (m - sqrt(3)*n)/(sqrt(3)*n) );

    qreal xe = xb - (p*h*0.75);
    qreal ye = yb - (t*h*0.75);

    QGraphicsLineItem *ab = new QGraphicsLineItem(xa,ya,xb,yb);
    QGraphicsLineItem *bc = new QGraphicsLineItem(xb,yb,xc,yc);
    QGraphicsLineItem *bd = new QGraphicsLineItem(xb,yb,xd,yd);
    QGraphicsLineItem *ec = new QGraphicsLineItem(xe,ye,xc,yc);
    QGraphicsLineItem *ed = new QGraphicsLineItem(xe,ye,xd,yd);

    ab->setPen(pen);
    bc->setPen(pen);
    bd->setPen(pen);
    ec->setPen(pen);
    ed->setPen(pen);

    arrowGroup->addToGroup(ab);
    arrowGroup->addToGroup(bc);
    arrowGroup->addToGroup(bd);
    arrowGroup->addToGroup(ec);
    arrowGroup->addToGroup(ed);

    return arrowGroup;
}

Sign GraphicsObject::detectSign(QString signStr)
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

QPointF GraphicsObject::detectPoint(QString pointStr)
{
    QPointF pf;
    QStringList strList = pointStr.split(',');
    if(strList.count() >= 2)
    {
        pf = QPointF(strList.at(0).toDouble(),strList.at(1).toDouble());
    }
    else
    {
        qDebug() << "Invalid point str list count " << pointStr << strList;
        pf = QPointF(0.0,0.0);
    }
    return pf;
}

void GraphicsObject::printLineEquotin(QPointF p1, QPointF p2, QPointF pt)
{
    qDebug() << "Start point: " << p1 << endl << "End point: " << p2 << endl << "Test Point: " << pt;
    qDebug() << "x - " << p1.rx() << "\t  y - " << p1.ry() << endl
             << "------\t= ------" << endl
             << " " << p2.rx() -p1.rx() << "\t     " << p2.ry() - p1.rx() << endl
             << "x = x_test = " << pt.rx() << ": " << ((pt.rx() - p1.rx()) / (p2.rx() - p1.rx())) << endl
             << "y = y_test = " << pt.ry() << ": " << ((pt.ry() - p1.ry()) / (p2.ry()-p1.ry())) << endl;
}


QDebug operator<<(QDebug os, const GraphicsObject &g)
{
    os.noquote() << "Image name: " << g.imgName() << endl
                 << "\tMicro feature ID: " << g.microFeatureID() << endl
                 << "\t\tType: " << g.printSign(g.type()) << " : " << g.startPoint() << " - " << g.endPoint();
    return os;
}

QDebug operator<<(QDebug os, GraphicsObject *g)
{
    os.noquote() << "Image name: " << g->imgName() << endl
                 << "\tMicro feature ID: " << g->microFeatureID() << endl
                 << "\t\tType: " << g->printSign( g->type() ) << " : " << g->startPoint() << " - " << g->endPoint();
    return os;
}
