#include "testserver.h"
#include "qasiotcpsocket.h"
#include <QDebug>

TestServer::TestServer(QObject *parent) : QObject(parent)
{
    connect(&server,&QAsioTcpServer::newConnection,this,&TestServer::newCon);
}

TestServer::~TestServer()
{
}

void TestServer::newCon(QAsioTcpSocket * socket)
{
    connect(socket,&QAsioTcpSocket::readReadly,this,&TestServer::readData);
    connect(socket,&QAsioTcpSocket::disconnected,socket,&QAsioTcpSocket::deleteLater);
}

void TestServer::readData()
{
    auto socket = qobject_cast<QAsioTcpSocket *>(sender());
    if (!socket) return;
    QByteArray data = socket->readAll();
    qDebug() << data;
    socket->write(data);
}
