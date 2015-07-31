#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "testserver.h"
#include "qasiotcpsocket.h"
#include <QThread>

TestServer::TestServer(QObject *parent) : QObject(parent)
{
    server.setThreadSize(3);
    connect(&server,&QAsioTcpServer::newConnection,this,&TestServer::newCon);
}

TestServer::~TestServer()
{
}

void TestServer::newCon(QAsioTcpsocket *socket)
{
    connect(socket,&QAsioTcpsocket::sentReadData,this,&TestServer::readData);
    myClient * clent = new myClient(socket,this);
    qDebug() << "NewCon! ID:" << clent->ID() << "\t" << QTime::currentTime().toString("HH:mm:ss.zzz");
}

void TestServer::readData(const QByteArray & data)
{
    qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz") << "\t  thread id :" << QThread::currentThreadId() << " \t the data: " << data;
}

myClient::myClient(QAsioTcpsocket * socket,QObject *parent) : QObject(parent),soc(socket)
{
    id = socket->socketDescriptor();
    connect(soc,&QAsioTcpsocket::sentReadData,this,&myClient::readData,Qt::DirectConnection);
    connect(soc,&QAsioTcpsocket::disConnected,this,&myClient::disConnected,Qt::QueuedConnection);
    connect(soc,&QAsioTcpsocket::erroString,this,&myClient::erroString,Qt::DirectConnection);
    QByteArray yy = "Hello Clinet!";
    socket->write(yy);
    soc->do_start();
}

myClient::~myClient()
{
    delete soc;
}


void myClient::readData(const QByteArray &data)
{
    soc->write(data);
    qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz") << "\t id:" << id <<
                " have data.\t thread id: " << QThread::currentThreadId();
}

void myClient::erroString(const QString &erro)
{
    qDebug() << QTime::currentTime().toString("hh:mm:ss.zzz") << "\t id:" << id <<
                " erro:"<< erro << "\t thread id: " << QThread::currentThreadId();
}

void myClient::disConnected()
{
    qDebug() << "Id:" << id << "disConnected \t " << QTime::currentTime().toString("HH:mm:ss.zzz");
    this->deleteLater();
}
