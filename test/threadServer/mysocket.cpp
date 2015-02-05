#include "mysocket.h"
#include <QTime>
#include <QDebug>

MySocket::MySocket(asio::ip::tcp::socket *socket, QObject *parent) :
    QAsioTcpSocket(socket,parent)
{
    connect(this,&MySocket::readReadly,this,&MySocket::HandleData);
    connect(this,&MySocket::disconnected,this,&MySocket::HandleDisCon);
}

MySocket::~MySocket()
{
}

void MySocket::HandleData()
{
    QByteArray by = readAll();
    write(by);
    qDebug() << "Thread ID:" << QThread::currentThreadId() << "\t\t"
           << QTime::currentTime().toString("HH:mm:ss.zzz")
           << "\t" << socketDescriptor() << "\t" << by;
}

void MySocket::HandleDisCon()
{
    emit sentDiscon(this);
}
