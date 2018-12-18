#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tlX_ = ui->tolLeftX_sb;
    tlY_ = ui->topLeftY_sb;
    w_ = ui->width_sb;
    h_ = ui->height_sb;
    gv_ = ui->graphicsView;
//    tlX_->setValue(0);
//    tlY_->setValue(0);
//    w_->setValue(150);
//    h_->setValue(80);
//    ui->angle_sb->setValue(20);
    QGraphicsScene *scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_arrowButton_clicked()
{
    QRectF rect = getUIRect();
    addItemToScene(rect, GT::Arrow);
}

void MainWindow::on_ellipseButton_clicked()
{
    QRectF rect = getUIRect();
    addItemToScene(rect, GT::Ellipse);
}

void MainWindow::on_squareButton_clicked()
{
    QRectF r = getUIRect();
    addItemToScene(r, GT::Square);
}

QRectF MainWindow::genRect(double x, double y, int w, int h)
{
    qreal xqr = static_cast<qreal>(x);
    qreal yqr = static_cast<qreal>(y);
    qreal wqr = static_cast<qreal>(w);
    qreal hqr = static_cast<qreal>(h);
    QRectF r(xqr, yqr, wqr, hqr);
    return r;
}

QGraphicsItemGroup *MainWindow::arrow(QRectF r)
{
    QGraphicsItemGroup *group = new QGraphicsItemGroup;

    QPointF ap = r.topLeft();
    QPointF bp = r.bottomRight();

    qreal xa = ap.rx();
    qreal xb = bp.rx();
    qreal ya = ap.ry();
    qreal yb = bp.ry();

    qreal n = yb - ya;
    qreal m = xb - xa;
    qreal len = sqrt( pow(n, 2) + pow(m, 2) );

    qreal t = n / len;
    qreal p = m / len;

    qreal h = len * 0.05;

    qreal xo = xb - (p*h);
    qreal yo = yb - (t*h);
    printLineEquotin(xo, xa, xb, yo, ya, yb);

    QGraphicsLineItem *ab = new QGraphicsLineItem(xa, ya, xb, yb);

    QPen pen = QPen(QColor(Qt::red), 1, Qt::SolidLine, Qt::RoundCap);
    ab->setPen(pen);
    group->addToGroup(ab);

    qreal xc = xb - (p*h)*( (m-n)/(sqrt(3) * m) );
    qreal yc = yb - (t*h)*( (sqrt(3)*n + m)/(sqrt(3) * n) );

    qreal xd = xb + (p*h)*( (m*(1 - 2*sqrt(3)) - n)/(sqrt(3) * m) );
    qreal yd = yb + (t*h)*( (m - sqrt(3)*n)/(sqrt(3)*n) );

    printLineEquotin(xo, xc, xd, yo, yc, yd);

    qreal xe = xb - (p*h*0.75);
    qreal ye = yb - (t*h*0.75);

    QGraphicsLineItem *bc = new QGraphicsLineItem(xb, yb, xc, yc);
    QGraphicsLineItem *bd = new QGraphicsLineItem(xb,yb,xd,yd);
    QGraphicsLineItem *ec = new QGraphicsLineItem(xe,ye,xc,yc);
    QGraphicsLineItem *ed = new QGraphicsLineItem(xe,ye,xd,yd);

    bc->setPen(pen);
    bd->setPen(pen);
    ec->setPen(pen);
    ed->setPen(pen);

    group->addToGroup(bc);
    group->addToGroup(bd);
    group->addToGroup(ec);
    group->addToGroup(ed);

    return group;
}

void MainWindow::updateGV()
{
    QRectF rect = getUIRect();
    GT t;
    if(ui->square_rb->isChecked())
    {
        t = GT::Square;
    }
    else if (ui->ellipse_rb->isChecked())
    {
        t = GT::Ellipse;
    }
    else if (ui->arrow_rb->isChecked())
    {
        t = GT::Arrow;
    }
    else
    {
        t = GT::Empty;
    }
    addItemToScene(rect, t);
}

QRectF MainWindow::getUIRect()
{
   double x = tlX_->value();
   double y = tlY_->value();
   int w = w_->value();
   int h = h_->value();
   return genRect(x, y, w, h);
}

void MainWindow::on_clearButton_clicked()
{
    clearScene();
}

void MainWindow::on_tolLeftX_sb_valueChanged(double arg1)
{
    updateGV();
}

void MainWindow::on_topLeftY_sb_valueChanged(double arg1)
{
    updateGV();
}

void MainWindow::on_width_sb_valueChanged(int arg1)
{
    updateGV();
}

void MainWindow::on_height_sb_valueChanged(int arg1)
{
    updateGV();
}

void MainWindow::clearScene()
{
    gv_->scene()->clear();
}

void MainWindow::addItemToScene(QRectF rect, GT gt)
{
    clearScene();
    QGraphicsScene *gs = gv_->scene();

    QGraphicsItemGroup *gIgr = new QGraphicsItemGroup;
    QAbstractGraphicsShapeItem *item;

    switch (gt)
    {
        case GT::Arrow:
        {
            gIgr = arrow( rect );
        }
            break;
        case GT::Ellipse:
        {
            item = new QGraphicsEllipseItem(rect);
            gIgr->addToGroup(item);
        }
            break;
        case GT::Square:
        {
            item = new QGraphicsRectItem(rect);
            gIgr->addToGroup(item);
        }
            break;
        case GT::Empty:
        {
            QGraphicsTextItem *textItem = new QGraphicsTextItem("NULL");
            gIgr->addToGroup(textItem);
            break;
        }
    }

    gs->addItem( gIgr);
    gv_->setScene(gs);

    QPointF ltl( rect.topLeft().x() - static_cast<qreal>( rect.width() * 0.1 ),
                 rect.topLeft().y() - static_cast<qreal>( rect.height() * 0.1 ));
    QSize ls( static_cast<int>(rect.width() * 2 ), static_cast<int>(rect.height() * 2));
    QRectF largeRect(ltl, ls);

    gv_->fitInView( largeRect );
}

void MainWindow::on_angle_sb_valueChanged(double arg1)
{
    updateGV();
}

void MainWindow::printLineEquotin(qreal x_test, qreal x1, qreal x2, qreal y_test, qreal y1, qreal y2)
{
    qDebug() <<endl << QPointF(x1,y1) << QPointF(x_test, y_test) << QPointF(x2,y2);
    qDebug() << "x - " << x1 << "  y - " << y1 << endl
             << "------ = ------" << endl
             << " " << x2 - x1 << "     " << y2 - y1 << endl
             << "x = x_test = " << x_test << ": " << ((x_test - x1) / (x2-x1)) << endl
             << "y = y_test = " << y_test << ": " << ((y_test - y1) / (y2-y1)) << endl;
}
