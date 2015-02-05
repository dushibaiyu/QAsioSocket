#ifndef THREADSERVER_H
#define THREADSERVER_H

#include "qasiotcpserver.h"
#include <QTime>
#include <QDebug>
#include <iostream>

class ThreadServer : public QAsioTcpServer
{
    Q_OBJECT
public:
    explicit ThreadServer(int threadsize = 2,int asioThread = -1 ,QObject *parent = 0);
    ~ThreadServer();

protected:
    void incomingConnection(QAsioTcpSocket *socket);

};

#endif // TESTSERVER_H
