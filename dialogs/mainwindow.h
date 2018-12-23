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
#include "db_connector.h"
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
    void treeModelChanged(QStandardItemModel* newModel);

public slots:
    void authAccepted();

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
    void on_microfeatures_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::MainWindow *ui;
    AuthDialog *authDialog_;
    void connectAll();
    DbConnector db_;

    QMap<int, PatientInfo> patients_;
    QVector<int> patientIDs_;
    int getIDByIndex(int index);
    int getIndexByID(int id);
    void changePatient(int patientID);
    QVector<GraphicsObject*> grObjectsVector_;

    void setMaxMin(QSpinBox *sb, int min = 0, int max = 0);
    void setSexRBs(const QString &sexStr);
};

#endif // MAINWINDOW_H
