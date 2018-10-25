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


namespace Ui {
class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = nullptr);
    ~AuthDialog();

private slots:
    void on_path_btn_clicked();
    void on_buttonBox_clicked(QAbstractButton *button);

signals:
    void authStatusChanged(bool acсessGranted);
    void rootPathChanged(QString pathToFold);

private:
    Ui::AuthDialog *ui;
    bool okOption(QString login, QString password);
    void closeOption();
    void resetOption();
};

#endif // AUTHDIALOG_H
