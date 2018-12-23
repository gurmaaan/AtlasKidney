#include "authdialog.h"
#include "ui_authdialog.h"


AuthDialog::AuthDialog(DbConnector& db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog),
    db(db)
{
    ui->setupUi(this);
    setWindowTitle(AUTH_DIALOG_TITTLE);

    // NOTE credentionals hard coding
    ui->login_le->setText(LOGIN);
    ui->password_le->setText(PASSWORD);

}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::on_path_btn_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, AUTH_CHOSE_FOLDER,
                                                QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                                                QFileDialog::ShowDirsOnly
                                                | QFileDialog::DontResolveSymlinks);
    setPath(dir);
}

void AuthDialog::setPath(QString path){
    emit basePathChanged(path);
    ui->path_le->setText(path);
}

void AuthDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    QList<QAbstractButton*> btnList = ui->buttonBox->buttons();
    if(button == btnList.at(0))
    {
        // Ok button action
        if(db.checkLoginPass(ui->login_le->text(), ui->password_le->text()) && QDir(ui->path_le->text()).exists() && ui->password_le->text().length() > 0)
        {            
            close();
            this->accept();
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
    bool accessGranted = ((ui->login_le->text() == login) && (ui->password_le->text() == password)) ? false : true;
    return accessGranted;
}

void AuthDialog::closeOption()
{
    close();
}

void AuthDialog::resetOption()
{
    ui->password_le->clear();
    ui->login_le->clear();
    ui->path_le->clear();
}
