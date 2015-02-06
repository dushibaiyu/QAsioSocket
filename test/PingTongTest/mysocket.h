#ifndef MYSOCKET_H
#define MYSOCKET_H

#include "qasiotcpsocketparent.h"
#include <QQueue>

class MyServer;

class MySocket : public QAsioTcpSocketParent
{
    Q_OBJECT
public:
    explicit MySocket(int size,QObject * parent = 0);
    ~MySocket();

    int readCount = 0;
    qint64 readSize = 0;
    int writeCount = 0;
    qint64 writeSize = 0;
protected:
    virtual bool write(const QByteArray &data);
    virtual void haveErro();
    virtual void hostConnected();
    virtual void readDataed(const char * data,std::size_t bytes_transferred);
    virtual bool writeDataed(std::size_t bytes_transferred);
    virtual void finedHosted();
protected:
    MySocket(asio::ip::tcp::socket * socket ,int size = 4096, QObject *parent = 0);
    friend class MyServer;
private:
    QQueue<QByteArray> writeQueue;
private:
    Q_DISABLE_COPY(MySocket)
};

#endif // MYSOCKET_H
