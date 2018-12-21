#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H

#include <QDebug>
#include <QPen>
#include <QGraphicsItemGroup>

enum class Sign
{
    Arrow,
    Ellipse,
    Square,
    NoSign
};


class GraphicsObject
{
public:
    GraphicsObject(QString imgName, int microFeatureID, Sign type, QPointF startPoint, QPointF endPoint);
    GraphicsObject(QStringList csvRow);
    GraphicsObject(const GraphicsObject &g);
    GraphicsObject() {}

    QString imgName() const;
    void setImgName(const QString &imgName);

    int microFeatureID() const;
    void setMicroFeatureID(int microFeatureID);
    void setMicroFeatureID(const QString &mfStr);

    Sign type() const;
    void setType(const Sign &type);
    void setType(const QString &typeStr);

    QPointF startPoint() const;
    void setStartPoint(const QPointF &startPoint);
    void setStartPoint(const QString &spStr);

    QPointF endPoint() const;
    void setEndPoint(const QPointF &endPoint);
    void setEndPoint(const QString &epStr);

    static QString printSign(const Sign &s);

    QGraphicsItemGroup *group() const;
    void setGroup(QGraphicsItemGroup *group);
    void calcGraphicsObject();

private:
    QString imgName_;
    int microFeatureID_;
    Sign type_;
    QPointF startPoint_;
    QPointF endPoint_;
    QGraphicsItemGroup *group_;
    QGraphicsItemGroup *genGrIGr(QPointF tlp, QPointF brp);
    QGraphicsItemGroup *genArrow(QPointF ap, QPointF bp, qreal angleSizeK);

    static Sign detectSign(QString signStr);
    static QPointF detectPoint(QString pointStr);
    void printLineEquotin(QPointF p1, QPointF p2, QPointF pt);
};

QDebug operator<< (QDebug os, const GraphicsObject& g);
QDebug operator<< (QDebug os, GraphicsObject* g);

#endif // GRAPHICSOBJECT_H
