#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <QObject>
#include "qasiotcpserver.h"
#include <QTime>
#include <QDebug>
#include <iostream>

class TestServer : public QObject
{
    Q_OBJECT
public:
    explicit TestServer(QObject *parent = 0);
    ~TestServer();

signals:

public slots:
    void listen(qint16 port = 2048) {
        qDebug() << "Listen  result:" <<  server.listen(port,QAsioTcpServer::IPV4);// << std::endl;
        startTime = QTime::currentTime();
        qDebug() << "Start Time:" << startTime;
    }
protected slots:
    void newCon(QAsioTcpSocket * socket);
    void readData();
private:
    QAsioTcpServer server;
    QTime startTime;
};

#endif // TESTSERVER_H
