#include "authdialog.h"
#include "ui_authdialog.h"
#include <QStandardPaths>

AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog)
{
    ui->setupUi(this);
    setWindowTitle("Авторизация");
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::on_path_btn_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    ui->path_le->setText(dir);
}

void AuthDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    QList<QAbstractButton*> btnList = ui->buttonBox->buttons();
    if(button == btnList.at(0))
    {
        // Ok button action
        qDebug() << "Ok";
        //Vanya ebash proverku i avtorizaciu zdes
        MainWindow mainWindowDialog;
        mainWindowDialog.show();
        //close();
    } else if (button == btnList.at(1))
    {
        //Close button action
        qDebug() << "Close";
        close();

    }else if (button == btnList.at(2))
    {
        //Reset button action
        ui->password_le->clear();
        ui->login_le->clear();
        ui->path_le->clear();
        qDebug() << "Reset";
    }
}
