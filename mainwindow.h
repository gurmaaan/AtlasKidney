#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <dialogs/authdialog.h>

#include "db_connector.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void enableMainWindow(bool authStatus);

private:
    Ui::MainWindow *ui;
    AuthDialog *authDialog;
    void connectAll();
    DB_connector db;
};

#endif // MAINWINDOW_H
