#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showMaximized();
    authDialog = new AuthDialog(db);
    authDialog->show();

    connectAll();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enableMainWindow(bool authStatus)
{
<<<<<<< HEAD
    if(authStatus)
    {
        qDebug() << "Eneble ,ain window GUI";
    }
=======
    //NOTE Dima skazal udalit'
//    if(authStatus)
//    {
//        ui->settingsDock->setEnabled(authStatus);
//        ui->label->setEnabled(authStatus);
//    }
>>>>>>> c0c25c3e0a989563f1aa13ee3c52fea2013acd29
}

void MainWindow::connectAll()
{
    connect(authDialog, &AuthDialog::authStatusChanged,
            this, &MainWindow::enableMainWindow);
}
