#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showMaximized();

    authDialog_ = new AuthDialog(db_);
    authDialog_->show();

    connectAll();

    //NOTE in reales it shouldn't be
    authDialog_->setPath(QString(IMG_PATH));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::authAccepted()
{
    setPatients(db_.getAllPatients());
    setPatientIDs(patients_);

    if(patients_.count() > 0)
    {
        int maxAge = 0;
        int maxDateFail = 0;
        for(PatientInfo pi : patients_)
        {
            if(pi.age() > maxAge)
                maxAge = pi.age();
            if(pi.dateOfFallIll() > maxDateFail)
                maxDateFail = pi.dateOfFallIll();
        }
        setMaxMin(ui->age_spin, 0, maxAge);
        setMaxMin(ui->failDate_spin, 0, maxDateFail);

        changePatient(getIDByIndex(0));
    }
}

void MainWindow::connectAll()
{
    connect(authDialog_, &AuthDialog::accepted,
            this, &MainWindow::authAccepted);
    connect(authDialog_, &AuthDialog::basePathChanged,
            ui->img_widget, &ImageWidget::setBasePath);
    connect(this, &MainWindow::imgNamesListChanged,
            ui->img_widget, &ImageWidget::setImgNames);
    connect(ui->enableFeaturesSigns_action, &QAction::toggled,
            ui->img_widget, &ImageWidget::drawSigns);
}

int MainWindow::getIDByIndex(int index)
{
    int id = 0;
    if(index <= patientIDs_.count() - 1)
        id = patientIDs_.at(index);
    else
        id = 0;
    return id;
}


int MainWindow::getIndexByID(int id)
{
    int index = 0;
    for(int i = 0; i < patientIDs_.count(); i++)
    {
        if(patientIDs_.at(i) == id)
            index = i;
    }
    return index;
}

void MainWindow::setPatientIDs(const QMap<int, PatientInfo> &patients)
{
    QVector<int> ids;
    for(int k : patients.keys())
        ids.append(k);

    patientIDs_ = ids;
    qDebug() << patientIDs_;

    setMaxMin(ui->currentPatient_sb, 0, ids.count() - 1);
    setMaxMin(ui->totalPatients_sb, 0, ids.count() - 1);
    ui->totalPatients_sb->setValue(ids.count() - 1);

    int min = ids.at(0);
    int max = min;
    for(int value : ids)
    {
        if(value < min)
            min = value;

        if(value > max)
            max = value;
    }
    setMaxMin(ui->id_spin, min, max);
}

void MainWindow::changePatient(int patientID)
{
    if(patients_.count() > 0)
    {
        PatientInfo& pi = patients_[patientID];
        ui->currentPatient_sb->setValue(getIndexByID(patientID));
        ui->id_spin->setValue(patientID);
        ui->medicalHistory_le->setText(pi.historyNum());
        ui->age_spin->setValue(pi.age());
        ui->failDate_spin->setValue(pi.dateOfFallIll());
        setSexRBs(QString(pi.sex()));
        QStringList images = pi.getImages().keys();

        if(!pi.getIsDownloadedFromDb())
            db_.updatePatientInfoById(patientID, pi);


        //FIXME for Dima. Some example of using new PatientInfo

//        qDebug() << pi.getImages().keys()[0];

//        qDebug() << "All features: " << (pi.getImages()["{7f34ce5b-796f-42b0-8408-e801f4bb88a4}.jpg"]).keys();

//        QVector<QPair<QString, QString>> features = pi.getImages()["{7f34ce5b-796f-42b0-8408-e801f4bb88a4}.jpg"][1817].features();
//        QVector<GraphicsObject> obj = pi.getImages()["{7f34ce5b-796f-42b0-8408-e801f4bb88a4}.jpg"][1817].objs();

//        qDebug() << "All features" << features;

        emit imgNamesListChanged(images);
    }
}

void MainWindow::setMaxMin(QSpinBox *sb, int min, int max)
{
    sb->setMinimum(min);
    sb->setMaximum(max);
}

void MainWindow::setSexRBs(const QString &sexStr)
{
    if(sexStr == SEX_F)
    {
        ui->sexF_radio->setChecked(true);
        ui->sexM_radio->setChecked(false);
    }
    else if(sexStr == SEX_M)
    {
        ui->sexF_radio->setChecked(false);
        ui->sexM_radio->setChecked(true);
    }
    else
    {
        ui->sexF_radio->setChecked(false);
        ui->sexM_radio->setChecked(false);
    }
}

void MainWindow::setPatients(const QMap<int, PatientInfo> &patients)
{
    patients_ = patients;
}

void MainWindow::on_nextpatient_action_triggered()
{
    int nextIndexID = ui->currentPatient_sb->value() + 1;
    if(nextIndexID <= ui->totalPatients_sb->value())
        changePatient(getIDByIndex(nextIndexID));
}

void MainWindow::on_previouspatient_action_triggered()
{
    int previousIndexID = ui->currentPatient_sb->value() - 1;
    if(previousIndexID >= 0)
        changePatient(getIDByIndex(previousIndexID));
}

void MainWindow::on_lastPatient_action_triggered()
{
    changePatient(getIDByIndex(ui->totalPatients_sb->value()));
}

void MainWindow::on_firstPatient_action_triggered()
{
    changePatient(getIDByIndex(0));
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

void MainWindow::on_microfeatures_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{

}
