#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QDebug>

#include "mainwindow.h"


class CustomLabel : public QLabel
{
Q_OBJECT
signals:
    void mousePressed( const QPoint& );

private:
    MainWindow* m_mainWindow;
    int m_w;
    int m_h;

public:
//    void set_w_h(int* w_h);
//    void setMainWindow(MainWindow* main);

    void setPath(QString path, QString fname) { this->path = path; this->fname = fname; first = true; }
    void setType(QString type) { this->type = type; first = true;}

    CustomLabel( QWidget* parent = nullptr, Qt::WindowFlags f = nullptr );

    CustomLabel( const QString& text, QWidget* parent = nullptr, Qt::WindowFlags f = nullptr );


protected:
    void mousePressEvent ( QMouseEvent* ev ) override;
    void mouseMoveEvent  ( QMouseEvent*  ) override;
    QString path;
    QString fname;
    QString type;
    QPoint point;
    bool first=true;
};


#endif // CUSTOMLABEL_H
