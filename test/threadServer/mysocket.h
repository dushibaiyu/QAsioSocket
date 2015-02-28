#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <QObject>
#include "qasiotcpsocket.h"

class ThreadServer;

class MySocket : public QAsioTcpSocket
{
    Q_OBJECT
public:
    ~MySocket();

signals:
    void sentDiscon(QThread *);
public slots:
    void HandleData();
    void HandleDisCon();
protected:
    explicit MySocket(int size = 4096, QObject *parent = 0);
    friend class ThreadServer;
    Q_DISABLE_COPY(MySocket)
};

#endif // MYSOCKET_H
