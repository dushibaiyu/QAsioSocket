#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "testserver.h"
#include "qasiotcpsocket.h"

TestServer::TestServer(QObject *parent) : QObject(parent)
{
    connect(&server,&QAsioTcpServer::newConnection,this,&TestServer::newCon);
}

TestServer::~TestServer()
{
}

void TestServer::newCon(QAsioTcpSocket * socket)
{
    qDebug() << "NewCon! ID:" << socket->socketDescriptor() << "\t\t\t" << QTime::currentTime().toString("HH:mm:ss.zzz");
    socket->setDisconnecdDeleteBuffer(true);
    connect(socket,&QAsioTcpSocket::readReadly,this,&TestServer::readData);
    connect(socket,&QAsioTcpSocket::disconnected,socket,&QAsioTcpSocket::deleteLater);
}

void TestServer::readData()
{
    auto socket = qobject_cast<QAsioTcpSocket *>(sender());
    if (!socket) return;
    QByteArray data = socket->readAll();
    socket->write(data);
    qDebug() << startTime.toString("HH:mm:ss.zzz") << "\t\t\t"
           << QTime::currentTime().toString("HH:mm:ss.zzz") << "\t" <<socket->socketDescriptor();
}
