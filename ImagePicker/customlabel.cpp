#include "customlabel.h"

#include <iostream>

void CustomLabel::mousePressEvent(QMouseEvent *ev){

    const QPoint p = ev->pos();

    if(first)
        point = QPoint(p.x(), p.y());
    else
        qDebug() << qUtf8Printable(path) << qUtf8Printable(fname) << type << p.x() << "," << p.y() << " " << p.x() << "," << p.y();

    first = !first;
    emit mousePressed( p );
}

void CustomLabel::mouseMoveEvent(QMouseEvent *) {
}
