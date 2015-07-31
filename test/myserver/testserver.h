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
        qDebug() << "Listen  result:" <<  server.listen(port);// << std::endl;
        startTime = QTime::currentTime();
        qDebug() << "Start Time:" << startTime;
    }
    void close() {
        server.close();
    }
protected slots:
    void newCon(QAsioTcpsocket * socket);
    void readData(const QByteArray & data);
private:
    QAsioTcpServer server;
    QTime startTime;
};

class myClient : public QObject
{
    Q_OBJECT
public:
    explicit myClient(QAsioTcpsocket * socket,QObject *parent = 0);
    ~myClient();

    int ID() const {return id;}
protected slots:
     void readData(const QByteArray & data);
     void erroString(const QString & erro);
     void disConnected();
private:
    QAsioTcpsocket * soc;
    int id;
};

#endif // TESTSERVER_H
