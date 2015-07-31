#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QAsioTcpsocket(4096,this);
    ui->pushSent->setEnabled(false);
    this->ui->timeBut->setEnabled(false);
    connect(socket,&QAsioTcpsocket::sentReadData,this,&MainWindow::readData);
    connect(socket,&QAsioTcpsocket::erroString,this,&MainWindow::readError);
    connect(&tm,&QTimer::timeout,[&](){
            int i = qrand() % 6;
            this->ui->textEdit->append(tr("%1 Timer Sent: %2").arg(QTime::currentTime().toString("hh:mm:ss.zzz")).arg(list.at(i)));
            socket->write(QByteArray(list.at(i).toUtf8()));
    });
    connect(socket,&QAsioTcpsocket::connected,this,&MainWindow::connectdd,Qt::QueuedConnection);
    connect(socket,&QAsioTcpsocket::disConnected,this,&MainWindow::disconnectdd);
    list << "我是谁?" << "渡世白玉" << "hello" << "哈哈哈哈哈" << "你是坏蛋!" <<  "测试一下下了" << "不知道写什么" ;
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    this->ui->txtIp->setText("127.0.0.1");
    this->ui->txtPort->setText("2048");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete socket;
    qDebug() << "MainWindow::~MainWindow()";
}

void MainWindow::on_pushConnect_clicked()
{
    qDebug() << "点击连接：" << socket->state();
    if ("连接" == this->ui->pushConnect->text())
    {
        QString ipAdd(this->ui->txtIp->text()), portd(this->ui->txtPort->text());
        if (ipAdd.isEmpty() || portd.isEmpty())
        {
            this->ui->textEdit->append("请输入IP和端口!");
            return;
        }
        socket->connectToHost(ipAdd,portd.toInt());
        this->ui->pushConnect->setEnabled(false);
    }
    else
    {
        socket->disconnectFromHost();
    }
}

void MainWindow::on_pushSent_clicked()
{
    qDebug() << "点击发送：" ;
    QString data = this->ui->txtData->text();
    if (data.isEmpty())
    {
        return ;
    }
    socket->write(QByteArray(data.toUtf8()));
    ui->textEdit->append(tr("Say：%1").arg(data));
}

void MainWindow::readError(const QString &erro)
{
    ui->pushConnect->setText("连接");
    ui->textEdit->append(tr("连接出错：%1").arg(erro));
    ui->pushSent->setEnabled(false);
    ui->pushConnect->setEnabled(true);
    this->ui->txtIp->setEnabled(true);
    this->ui->txtPort->setEnabled(true);
    tm.stop();
    this->ui->timeBut->setEnabled(false);
    this->ui->lineEdit->setEnabled(true);
    this->ui->timeBut->setText("启动定时");
}

void MainWindow::connectdd()
{
    ui->pushConnect->setText("断开");
    ui->textEdit->append("连接服务器成功");
    ui->pushSent->setEnabled(true);
    this->ui->txtIp->setEnabled(false);
    this->ui->txtPort->setEnabled(false);
    this->ui->timeBut->setEnabled(true);
    ui->pushConnect->setEnabled(true);
}

void MainWindow::disconnectdd()
{
    qDebug()<< "QTcpSocket::disconnected" ;
    ui->pushConnect->setText("连接");
    ui->textEdit->append("断开服务器");
    ui->pushSent->setEnabled(false);
    this->ui->txtIp->setEnabled(true);
    this->ui->txtPort->setEnabled(true);
    this->ui->timeBut->setEnabled(false);
    this->ui->lineEdit->setEnabled(true);
    this->ui->timeBut->setText("启动定时");
    this->tm.stop();
}

void MainWindow::readData(const QByteArray & data)
{
    this->ui->textEdit->append(tr("%1 Server Say：%2").arg(QTime::currentTime().toString("hh:mm:ss.zzz"))
                               .arg(QString(data)));
}

void MainWindow::on_timeBut_clicked()
{
    if (this->ui->lineEdit->text().isEmpty())
    {
        this->ui->textEdit->append("请输入时间：");
        return;
    }
    if ("启动定时" == this->ui->timeBut->text())
    {
        int h = this->ui->lineEdit->text().toInt();
        h = h*1000;
        tm.start(h);
        this->ui->lineEdit->setEnabled(false);
        this->ui->timeBut->setText("停止定时");
    }
    else
    {
        tm.stop();
        this->ui->timeBut->setText("启动定时");
        this->ui->lineEdit->setEnabled(true);
    }
}
