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
    if(authStatus)
    {
        qDebug() << "Eneble ,ain window GUI";
    }
}

void MainWindow::connectAll()
{
    connect(authDialog, &AuthDialog::authStatusChanged,
            this, &MainWindow::enableMainWindow);
}
