#include "authdialog.h"
#include "ui_authdialog.h"


AuthDialog::AuthDialog(DB_connector& db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog),
    db(db)
{
    ui->setupUi(this);
    setWindowTitle("Авторизация");

    // NOTE
    ui->login_le->setText("admin_db");
    ui->password_le->setText("admin_db_");
    ui->path_le->setText("C:/Users/Admin/Downloads");
//    ui->buttonBox->buttons()[0]->click();
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
    emit rootPathChanged(dir);
    ui->path_le->setText(dir);
}

void AuthDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    QList<QAbstractButton*> btnList = ui->buttonBox->buttons();
    if(button == btnList.at(0))
    {
        // Ok button action

        //TODO: Vanya ebash proverku i avtorizaciu zdes
        //

        qDebug() << ui->path_le->text();
        if(db.check_login_pass(ui->login_le->text(), ui->password_le->text()) && QDir(ui->path_le->text()).exists() && ui->path_le->text().length() > 0)
        {            
            emit authStatusChanged(true);
            close();
        }
        else
        {
            resetOption();
            QMessageBox::critical(nullptr, AUTH_MSG_TITTLE, AUTH_MSG_BODY);
        }
    } else if (button == btnList.at(1))
    {
        //Close button action
        closeOption();
    }else if (button == btnList.at(2))
    {
        //Reset button action
        resetOption();
    }
}

bool AuthDialog::okOption(QString login, QString password)
{
    qDebug() << "Ok";
    bool accessGranted = ((ui->login_le->text() == login) && (ui->path_le->text() == password)) ? false : true;
    return accessGranted;
}

void AuthDialog::closeOption()
{
    qDebug() << "Close";
    close();
}

void AuthDialog::resetOption()
{
    ui->password_le->clear();
    ui->login_le->clear();
    ui->path_le->clear();
    qDebug() << "Reset";
}
