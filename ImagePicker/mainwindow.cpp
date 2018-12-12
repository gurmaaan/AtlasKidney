#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBox->addItem("Arrow");
    ui->comboBox->addItem("Ellipse");
    ui->comboBox->addItem("Square");

    ui->label->setType("Arrow");

    home_dir = QFileDialog::getExistingDirectory(this, tr("Open file"), "C://");

    QDir home(home_dir);
    dirs = home.entryList(QDir::Filter::Dirs);

    dirs.erase(dirs.begin());
    dirs.erase(dirs.begin());
    files = QDir(home_dir + "/" + dirs[0]).entryList(QDir::Filter::Files);

    setImage(home_dir + "/" + dirs[0] + "/" + files[0]);
    ui->label->setPath(dirs[0], files[0]);
    files.erase(files.begin());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setImage(QString path)
{

    ui->label->setPixmap(QPixmap(path));
}

void MainWindow::on_pushButton_clicked()
{

    if (files.size() != 0){
        setImage(home_dir + "/" + dirs[0] + "/" + files[0]);
        ui->label->setPath(dirs[0], files[0]);
        files.erase(files.begin());
        return;
    }
    if (dirs.size() == 0)
        exit(0);
    dirs.erase(dirs.begin());
    files = QDir(home_dir + "/" + dirs[0]).entryList(QDir::Filter::Files);

    setImage(home_dir + "/" + dirs[0] + "/" + files[0]);
    ui->label->setPath(dirs[0], files[0]);

    files.erase(files.begin());

}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    ui->label->setType(arg1);
}
