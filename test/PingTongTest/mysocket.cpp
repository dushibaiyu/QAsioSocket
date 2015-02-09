#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "mysocket.h"
#include <QDebug>
#include <QTime>
#include <QFile>

MySocket::MySocket(int size, QObject *parent) : QAsioTcpSocketParent(size,parent)
{}

MySocket::MySocket(asio::ip::tcp::socket * socket , int size, QObject *parent)
    : QAsioTcpSocketParent(socket,size,parent)
{
}

MySocket::~MySocket()
{
    if (socketDescriptor() != -1)
        disconnectFromHost();
}

void MySocket::haveErro()
{
//    this->deleteLater();
}

void MySocket::hostConnected()
{
    QByteArray data(4096,'a');
//    qDebug() << data;
    write(data);
}

void MySocket::readDataed(const char * data,std::size_t bytes_transferred)
{
    QByteArray by(data,bytes_transferred);
    readSize += bytes_transferred;
    readCount ++;
    write(by);
}

bool MySocket::writeDataed(std::size_t bytes_transferred)
{
    if (static_cast<std::size_t>(writeQueue.head().size()) == bytes_transferred){
        writeCount ++;
        writeSize += bytes_transferred;
        writeQueue.dequeue();
        if (!writeQueue.isEmpty())
            write(QByteArray());
        return true;
    } else {
        return false;
    }
}

void MySocket::finedHosted()
{
}


bool MySocket::write(const QByteArray &data)
{
    if (state() != ConnectedState)
        return false;
    if (!data.isEmpty())
        writeQueue.append(data);
    if (writeQueue.isEmpty()) {
        return false;
    }
    wirteData(writeQueue.head().data(),static_cast<std::size_t>(writeQueue.head().size()));
    return true;
}
