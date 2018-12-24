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

    twg_ = ui->microF_twg;
    connectAll();
    setupTreeWidget();
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

void MainWindow::selectedImageIndexChanged(int index)
{
    twg_->collapseAll();
    QString selectedItemText = "Изображение " + QString::number(index + 1);
    QTreeWidgetItem *indexItem = twg_->topLevelItem(index);
    if(indexItem->text(0) == selectedItemText)
        indexItem->setExpanded(true);
    else
    {
        for(int i = 0; i < twg_->topLevelItemCount(); i++)
        {
            QTreeWidgetItem *itemAtI = twg_->topLevelItem(i);
            if(itemAtI->text(0) == selectedItemText)
                itemAtI->setExpanded(true);
        }
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

    connect(ui->img_widget, &ImageWidget::selectedIndexChanged,
            this, &MainWindow::selectedImageIndexChanged);
}

void MainWindow::setupTreeWidget()
{
    twg_->setHeaderLabels( QStringList() <<"Изображение" << "Микропризнак" << "Описание" );
    twg_->setSortingEnabled(true);
    int cw = twg_->width() / 3;
    twg_->setColumnWidth(0, (cw * 1.25));
    twg_->setColumnWidth(1, cw);
    twg_->setColumnWidth(2, cw);
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
        if(!pi.getIsDownloadedFromDb())
            db_.updatePatientInfoById(patientID, pi);
        ui->currentPatient_sb->setValue(getIndexByID(patientID));
        ui->id_spin->setValue(patientID);
        ui->medicalHistory_le->setText(pi.historyNum());
        ui->age_spin->setValue(pi.age());
        ui->failDate_spin->setValue(pi.dateOfFallIll());

        setSexRBs(QString(pi.sex()));

        QMap<QString, QMap<int, Feature>> images = pi.getImages();
        QStringList imgNamesStrList = images.keys();
        ui->imgCnt_sb->setValue(imgNamesStrList.count());
        emit imgNamesListChanged(imgNamesStrList);
        
        twg_->clear();
        for(int i = 0; i < imgNamesStrList.count(); i++)
        {
            QTreeWidgetItem *imgItem = new QTreeWidgetItem(twg_);
            imgItem->setText( 0, "Изображение " + QString::number(i+1) );
            QString imgNameAtI = imgNamesStrList.at(i);
            imgItem->setStatusTip( 0, imgNameAtI );
            imgItem->setWhatsThis( 0, imgNameAtI );

            QMap<int, Feature> imgMasks = images[imgNameAtI];
            if(imgMasks.keys().count() > 0)
            {
                for(int maskID : imgMasks.keys())
                {
                    QTreeWidgetItem *imgMaskItem = new QTreeWidgetItem(imgItem);
                    imgMaskItem->setText(0, "Маска " + QString::number(maskID));
                    QVector< QPair<QString, QString> > features = imgMasks[maskID].features();
                    QVector< GraphicsObject > grObjs = imgMasks[maskID].objs();
                    if( !grObjs.empty() )
                    {
                        qDebug() << "Not empty: " << patientID << maskID << imgNameAtI;
                        imgMaskItem->setCheckState(0, Qt::Unchecked);
                        for(GraphicsObject go : grObjs)
                        {
                        }
                    }
                    if( !features.empty() )
                    {
                        for(QPair<QString,QString> f : features)
                        {
                            QTreeWidgetItem *featureItem = new QTreeWidgetItem(imgMaskItem);
                            featureItem->setText(1, f.first);
                            featureItem->setText(2, f.second);
                        }
                    }

                }
            }
        }
        twg_->sortByColumn(0, Qt::AscendingOrder);
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

void MainWindow::on_microF_twg_itemClicked(QTreeWidgetItem *item, int column)
{
}

QVector<GraphicsObject> MainWindow::getPatientsGrObjVector(int pID, QString imageName, int maskID)
{
    PatientInfo pi = patients_[pID];
    QMap<int, Feature> imgMasks = pi.getImages()[imageName];
    QVector< GraphicsObject > grObjs = imgMasks[maskID].objs();
    return grObjs;
}

void MainWindow::on_microF_twg_itemChanged(QTreeWidgetItem *item, int column)
{

}

void MainWindow::on_collapseAll_btn_clicked()
{
    twg_->collapseAll();
}

void MainWindow::on_expandAll_btn_clicked()
{
    twg_->expandAll();
}
