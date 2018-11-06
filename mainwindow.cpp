#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    showMaximized();
    authDialog = new AuthDialog(db_);
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
        qDebug() << "Eneble ,main window GUI";
    }
}

void MainWindow::authAccepted()
{
    setPatients(db_.get_all_patients());

    ui->id_spin->setMaximum(patients_.size());
    ui->id_spin->setMinimum(0);

    int maxAge = 0;
    for(Patient_info pi : patients_)
    {
        if(pi.age() > maxAge)
            maxAge = pi.age();
    }
    qDebug() << maxAge;
    ui->age_spin->setMaximum(0);
    ui->age_spin->setMaximum(maxAge);

    changePatient(0);
}
void MainWindow::connectAll()
{
    connect(authDialog, &AuthDialog::authStatusChanged,
            this, &MainWindow::enableMainWindow);
    connect(authDialog, &AuthDialog::accepted,
            this, &MainWindow::authAccepted);
}

void MainWindow::changePatient(int patientID)
{
    ui->medicalHistory_le->setText(patients_.at(patientID).historyNum());
    ui->age_spin->setValue(patients_.at(patientID).age());
}

void MainWindow::setPatients(const QVector<Patient_info> &patients)
{
    patients_ = patients;
}

void MainWindow::on_nextpatient_action_triggered()
{
    ui->id_spin->setValue( ui->id_spin->value() + 1);
    changePatient(ui->id_spin->value());
}

void MainWindow::on_previouspatient_action_triggered()
{
    ui->id_spin->setValue( ui->id_spin->value() - 1);
    changePatient(ui->id_spin->value());
}
