#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "threadserver.h"
#include "qasiotcpsocket.h"

ThreadServer::ThreadServer(int threadsize,int asioThread ,QObject *parent)
    : QAsioTcpServer(asioThread,parent)
{
    connect(&server,&QAsioTcpServer::newConnection,this,&TestServer::newCon);
}

ThreadServer::~ThreadServer()
{
}

void ThreadServer::newCon(QAsioTcpSocket * socket)
{
    qDebug() << "NewCon! ID:" << socket->socketDescriptor() << "\t\t\t" << QTime::currentTime().toString("HH:mm:ss.zzz");
    socket->setDisconnecdDeleteBuffer(true);
    connect(socket,&QAsioTcpSocket::readReadly,this,&TestServer::readData);
    connect(socket,&QAsioTcpSocket::disconnected,socket,&QAsioTcpSocket::deleteLater);
}

void ThreadServer::readData()
{
    auto socket = qobject_cast<QAsioTcpSocket *>(sender());
    if (!socket) return;
    QByteArray data = socket->readAll();
    socket->write(data);
    qDebug() << startTime.toString("HH:mm:ss.zzz") << "\t\t\t"
           << QTime::currentTime().toString("HH:mm:ss.zzz") << "\t" <<socket->socketDescriptor();
}
