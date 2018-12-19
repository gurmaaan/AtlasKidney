#include "customlabel.h"

#include <iostream>
#include <QJsonDocument>

CustomLabel::CustomLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel( parent, f ) { }

CustomLabel::CustomLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel( text, parent, f ) { }

void CustomLabel::mousePressEvent(QMouseEvent *ev){

    const QPoint p = ev->pos();

    if(first)
        point = QPoint(p.x(), p.y());
    else
        qDebug() << qUtf8Printable(path) << qUtf8Printable(fname) << type << point.x() << "," << point.y() << " " << p.x() << "," << p.y();

    first = !first;
    emit mousePressed( p );
}

void CustomLabel::mouseMoveEvent(QMouseEvent *) {
}
