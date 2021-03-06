#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
//
#include "static.h"
#include "databaseservice.h"


namespace Ui {
class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(DataBaseService& db, QWidget *parent = nullptr);
    //NOTE in reales it should be in private
    void setPath(QString path);
    ~AuthDialog();

private slots:
    void on_path_btn_clicked();
    void on_buttonBox_clicked(QAbstractButton *button);

signals:
    void basePathChanged(QString pathToFold);

private:
    Ui::AuthDialog *ui;
    bool okOption(QString login, QString password);
    void closeOption();
    void resetOption();
    DataBaseService db;
};

#endif // AUTHDIALOG_H
