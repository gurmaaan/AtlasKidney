#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QAbstractGraphicsShapeItem>
#include <QSpinBox>
#include <QDebug>

enum class GT
{
    Arrow,
    Ellipse,
    Square,
    Empty
};
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QGraphicsScene *scene() const;
    void setScene(QGraphicsScene *scene);
    QRectF getUIRect();
    void addItemToScene(QRectF rect, GT gt);

public slots:


private slots:
    void on_arrowButton_clicked();
    void on_ellipseButton_clicked();
    void on_squareButton_clicked();
    void on_clearButton_clicked();
    void on_tolLeftX_sb_valueChanged(double arg1);
    void on_topLeftY_sb_valueChanged(double arg1);
    void on_width_sb_valueChanged(int arg1);
    void on_height_sb_valueChanged(int arg1);

    void on_angle_sb_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
    QDoubleSpinBox *tlX_;
    QDoubleSpinBox *tlY_;
    QSpinBox *w_;
    QSpinBox *h_;
    QGraphicsView *gv_;
    void printLineEquotin(qreal x_test, qreal x1, qreal x2, qreal y_test, qreal y1, qreal y2);

    void clearScene();
    QRectF genRect(double x, double y, int w, int h);
    QGraphicsItemGroup* arrow(QRectF r);
    void updateGV();
};

#endif // MAINWINDOW_H
