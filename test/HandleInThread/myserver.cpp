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

bool MyServer::haveErro()
{
    return true;
}

void MyServer::incomingConnection()
{
    MySocket * asiosocket = new MySocket(bsize);
    if (setNewSocket(asiosocket)) {
        asiosocket->moveToThread(this->thread());
        handler.add(asiosocket);
        qDebug() << "Thread Id:" << QThread::currentThreadId() << "\t\t"
                 << "HaveNewConnecd ID:" << asiosocket->socketDescriptor();
    } else {
        delete asiosocket;
    }
}
