#ifndef THREADSERVER_H
#define THREADSERVER_H

#include "qasiotcpserver.h"
#include <QTime>
#include <QDebug>
#include <iostream>
#include "threadhandle.h"

class MySocket;

class ThreadServer : public QAsioTcpServer
{
    Q_OBJECT
public:
    explicit ThreadServer(int threadsize = 2, QObject *parent = 0);
    ~ThreadServer();

public slots:
    void removeThread(QThread * );

protected:
    void incomingConnection();

private:
    ThreadHandle handler;
};

#endif // TESTSERVER_H
