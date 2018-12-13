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
        //TODO: либо убрать этот метод либо сделать энейбл/дисейбл гуй
        qDebug() << "Enable ,main window GUI";
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
    connect(authDialog, &AuthDialog::basePathChanged,
            ui->img_widget, &ImageWidget::setBasePath);
    connect(this, &MainWindow::imgNamesListChanged,
            ui->img_widget, ImageWidget::setImgNames);
    connect(ui->enableFeaturesSigns_action, &QAction::toggled,
            ui->img_widget, &ImageWidget::drawSigns);
}

void MainWindow::changePatient(int patientID)
{
    PatientInfo pi = patients_.at(patientID);
    ui->id_spin->setValue(patientID);
    ui->medicalHistory_le->setText(pi.historyNum());
    ui->age_spin->setValue(pi.age());
    ui->failDate_spin->setValue(pi.dateOfFallIll());
    if(QString(pi.sex()) == "F")
        ui->sexF_radio->setChecked(true);
    else if(QString(pi.sex()) == "M")
        ui->sexM_radio->setChecked(true);
    else {
        ui->sexF_radio->setChecked(false);
        ui->sexM_radio->setChecked(false);
    }
    QVector<ImageInfo> images = pi.images();
    QStringList imgList;
    for (ImageInfo i: images)
        imgList.push_back(i.path());

    emit imgNamesListChanged(imgList);

    ui->microfeatures_listView->reset();
//    QStandardItemModel *listModel = new QStandardItemModel();
//    QStringList microFeaturesList = pi.macroFeatures();
//    for(QString feature : microFeaturesList)
//    {
//        QStandardItem *listItem = new QStandardItem(feature);
//        listModel->appendRow(listItem);
//    }
//    ui->microfeatures_listView->setModel(listModel);
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

void MainWindow::on_quit_action_triggered()
{
    QApplication::quit();
}

void MainWindow::on_github_action_triggered()
{
    QString link(GURMAAAN);
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_fullscreen_action_triggered()
{
    ui->img_widget->on_fullscreen_toolbtn_clicked();
}

void MainWindow::on_disconnect_action_triggered()
{
    //TODO
}

void MainWindow::on_devmanual_action_triggered()
{
    //TODO open pdf with guide
}

void MainWindow::on_zoom_out_action_triggered()
{
    ui->img_widget->on_minus_toolbtn_clicked();
}

void MainWindow::on_zoom_in_action_triggered()
{
    ui->img_widget->on_plus_toolbtn_clicked();
}

void MainWindow::on_usermanual_action_triggered()
{
   //TODO open pdf with manual
}
