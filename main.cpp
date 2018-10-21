#include "mainwindow.h"
#include "dialogs/authdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AuthDialog authD;
    authD.show();

    return a.exec();
}
