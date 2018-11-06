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
    setPatients(db_.getAllPatients());

    ui->id_spin->setMaximum(patients_.size());
    ui->id_spin->setMinimum(0);

    int maxAge = 0;
    int maxDateFail = 0;
    for(PatientInfo pi : patients_)
    {
        if(pi.age() > maxAge)
            maxAge = pi.age();
        if(pi.dateOfFallIll() > maxDateFail)
            maxDateFail = pi.dateOfFallIll();
    }

    ui->age_spin->setMaximum(0);
    ui->age_spin->setMaximum(maxAge);

    ui->failDate_spin->setMinimum(0);
    ui->failDate_spin->setMaximum(maxDateFail);

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
    PatientInfo pi = patients_.at(patientID);
    ui->id_spin->setValue(patientID);
    ui->medicalHistory_le->setText(pi.historyNum());
    ui->age_spin->setValue(pi.age());
    ui->failDate_spin->setValue(pi.dateOfFallIll());

    QStringList imgList = pi.imagesPaths();
    ui->imgList_listView->reset();
    QStandardItemModel *listModel = new QStandardItemModel();
    for(QString imgName : imgList)
    {
        QStandardItem *listItem = new QStandardItem(imgName);
        listModel->appendRow(listItem);
    }
    ui->imgList_listView->setModel(listModel);
}

void MainWindow::setPatients(const QVector<PatientInfo> &patients)
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

void MainWindow::on_lastPatient_action_triggered()
{
    changePatient(patients_.size()-1);
}

void MainWindow::on_firstPatient_action_triggered()
{
    changePatient(0);
}
