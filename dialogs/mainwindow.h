#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTreeWidgetItem>
#include <QListView>
#include <QDesktopServices>
#include <QSpinBox>
#include <type_traits>

#include <dialogs/authdialog.h>
#include <widgets/imagewidget.h>
#include <static.h>
#include "databaseservice.h"
#include "model/graphicsobject.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setPatients(const QMap<int, PatientInfo> &patients);
    void setPatientIDs(const QMap<int, PatientInfo> &patients);

signals:
    void imgNamesListChanged(QStringList &imgNames);

public slots:
    void authAccepted();
    void selectedImageIndexChanged(int index);

private slots:
    void on_nextpatient_action_triggered();
    void on_previouspatient_action_triggered();
    void on_lastPatient_action_triggered();
    void on_firstPatient_action_triggered();
    void on_quit_action_triggered();
    void on_github_action_triggered();
    void on_fullscreen_action_triggered();
    void on_devmanual_action_triggered();
    void on_zoom_out_action_triggered();
    void on_zoom_in_action_triggered();
    void on_usermanual_action_triggered();
    void on_microF_twg_itemChanged(QTreeWidgetItem *item, int column);
    void on_collapseAll_btn_clicked();
    void on_expandAll_btn_clicked();

private:
    Ui::MainWindow *ui;
    QTreeWidget *twg_;
    AuthDialog *authDialog_;
    DataBaseService db_;

    QMap<int, PatientInfo> patients_;
    QVector<int> patientIDs_;
    QVector<GraphicsObject> getPatientsGrObjVector(int pID, QString imageName, int maskID);

    int getIDByIndex(int index);
    int getIndexByID(int id);
    void changePatient(int patientID);

    QTreeWidgetItem *createImgItem(QTreeWidget *root, int i, QString realName);
    void createMaskItem(QTreeWidgetItem* imgItem, QMap<int, Feature> masks, int maskID);
    void setMaxMin(QSpinBox *sb, int min = 0, int max = 0);
    void setSexRBs(const QString &sexStr);
    void connectAll();
    void setupTreeWidget();
};

#endif // MAINWINDOW_H
