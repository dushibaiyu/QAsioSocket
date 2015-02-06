#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "threadserver.h"
#include "qasiotcpsocket.h"
#include "mysocket.h"
#include <QThread>
#include <QDebug>


ThreadServer::ThreadServer(int threadsize,int asioThread ,QObject *parent)
    : QAsioTcpServer(threadsize,asioThread,parent)
{
    qDebug() << "Main Thread Id :" << QThread::currentThreadId();
    handler.initThreadType(ThreadHandle::THREADSIZE,threadsize);
}

ThreadServer::~ThreadServer()
{
}


void ThreadServer::incomingConnection(asio::ip::tcp::socket *socket)
{
    MySocket * soc = new MySocket(socket);
    soc->moveToThread(handler.getThread());
    QObject::connect(soc,&MySocket::sentDiscon,this,&ThreadServer::removeThread);
    qDebug() << "New Connect Id:" << soc->socketDescriptor() << "\t\t"
             << "in Thread :" << QThread::currentThreadId();
}

void ThreadServer::removeThread(QThread * thread)
{
    handler.removeThread(thread);
}
