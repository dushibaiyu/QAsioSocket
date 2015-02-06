#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    connect(&socket,&sockets::sentData,this,&Form::haveData);
    connect(&socket,&sockets::sentEnd,[&](){this->ui->pushButton->setEnabled(true);});
}

Form::~Form()
{
    delete ui;
}

void Form::on_pushButton_clicked()
{
    QString Ip = this->ui->lineIp->text();
    if (Ip.isEmpty()) return;
    this->ui->show->clear();
    this->ui->pushButton->setEnabled(false);
    socket.Creator(this->ui->size->value());
    socket.start(this->ui->time->value(),Ip,static_cast<qint16>(this->ui->port->value()));
}

void Form::haveData(int site ,int revCou, int sentCou,qint64 revSize,qint64 sentSize)
{
    if (site == 0) {
        this->ui->tel->setText(tr("%1秒一共接受%2次，发送%3次，接受%4字节，发送%5字节。")
                               .arg(this->ui->time->value()).arg(revCou).arg(sentCou)
                               .arg(revSize).arg(sentSize));
    } else {
        this->ui->show->append(tr("%1:接受%2次，发送%3次，接受%4字节，发送%5字节。")
                               .arg(site).arg(revCou).arg(sentCou).arg(revSize).arg(sentSize));
    }
}
