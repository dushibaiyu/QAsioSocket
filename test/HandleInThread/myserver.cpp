#include "myserver.h"
#include "mysocket.h"
#include <QDebug>
#include <QThread>

MyServer::MyServer(int thsize, int buffersize, QObject *parent)
    : QAsioTcpServerParent(thsize,parent),bsize(buffersize)
{
    qDebug() << "Main Thread Id:" << QThread::currentThreadId();
}

MyServer::~MyServer()
{

}

bool MyServer::haveErro(const asio::error_code & /*code*/)
{
    return true;
}

void MyServer::incomingConnection(asio::ip::tcp::socket * socket)
{
    MySocket * asiosocket = new MySocket(socket,bsize);
    asiosocket->moveToThread(this->thread());
    handler.add(asiosocket);
    qDebug() << "Thread Id:" << QThread::currentThreadId() << "\t\t"
             << "HaveNewConnecd ID:" << asiosocket->socketDescriptor();
}
