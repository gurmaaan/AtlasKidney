#include "dialogs/mainwindow.h"
#include "dialogs/authdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainD;
    mainD.show();
    return a.exec();
}
