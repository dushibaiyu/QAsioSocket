#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->pushButton_2->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
     this->ui->pushButton_2->setEnabled(true);
    this->ui->pushButton->setEnabled(false);
   server.listen();
}

void MainWindow::on_pushButton_2_clicked()
{
   server.close();
   this->ui->pushButton_2->setEnabled(false);
   this->ui->pushButton->setEnabled(true);
}
