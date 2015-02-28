#ifndef MYSOCKET_H
#define MYSOCKET_H

#include "qasiotcpsocketparent.h"
#include <QQueue>

class MyServer;

class MySocket : public QAsioTcpSocketParent
{
    Q_OBJECT
public:
    explicit MySocket(int size = 4096,QObject * parent = 0);
    ~MySocket();

protected:
    virtual bool write(const QByteArray &data);
    virtual void haveErro();
    virtual void hostConnected();
    virtual void readDataed(const char * data,std::size_t bytes_transferred);
    virtual bool writeDataed(std::size_t bytes_transferred);
    virtual void finedHosted();
private:
    QQueue<QByteArray> writeQueue;
private:
    Q_DISABLE_COPY(MySocket)
};

#endif // MYSOCKET_H
