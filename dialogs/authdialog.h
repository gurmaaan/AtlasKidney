#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QDialogButtonBox>
#include <QDebug>
#include <QFileDialog>
//
#include "mainwindow.h"

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

private:
    Ui::AuthDialog *ui;
};

#endif // AUTHDIALOG_H
