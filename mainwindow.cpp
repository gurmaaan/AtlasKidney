#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showMaximized();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::enableMainWindow(bool authStatus)
{
    if(authStatus)
    {
        ui->settingsDock->setEnabled(authStatus);
        ui->label->setEnabled(authStatus);
    }
}
