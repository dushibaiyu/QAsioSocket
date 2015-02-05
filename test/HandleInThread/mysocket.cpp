#include "mysocket.h"
#include <QDebug>
#include <QTime>

MySocket::MySocket(QObject *parent) : QAsioTcpSocketParent(parent)
{}

MySocket::MySocket(asio::ip::tcp::socket * socket , QObject *parent)
    : QAsioTcpSocketParent(socket,parent)
{

}

MySocket::~MySocket()
{
    if (socketDescriptor() != -1)
        disconnectFromHost();
}

void MySocket::haveErro()
{
    this->deleteLater();
}

void MySocket::hostConnected()
{
}

void MySocket::readDataed(const char * data,std::size_t bytes_transferred)
{
    QByteArray by(data,bytes_transferred);
    write(by);
    qDebug() << "Thread ID:" << QThread::currentThreadId() << "\t\t"
           << QTime::currentTime().toString("HH:mm:ss.zzz")
           << "\t" << socketDescriptor() << "\t" << by;
}

bool MySocket::writeDataed(std::size_t bytes_transferred)
{
    if (static_cast<std::size_t>(writeQueue.head().size()) == bytes_transferred){
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
