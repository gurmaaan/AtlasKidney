#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
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

    void setPatients(const QVector<Patient_info> &patients);

public slots:
    void enableMainWindow(bool authStatus);
    void authAccepted();

private slots:
    void on_nextpatient_action_triggered();
    void on_previouspatient_action_triggered();

private:
    Ui::MainWindow *ui;
    AuthDialog *authDialog;
    void connectAll();
    DB_connector db_;
    QVector<Patient_info> patients_;
    void changePatient(int patientID);
};

#endif // MAINWINDOW_H
