#ifndef IOSERVERTHREAD_H
#define IOSERVERTHREAD_H

#ifdef Q_OS_WIN
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include "include_asio/asio.hpp"
#include <QThread>

class IOServerThread : public QThread
{
    Q_OBJECT
public:
    explicit IOServerThread(QObject *parent = 0);
    ~IOServerThread();

public:
    asio::io_service & getIOServer(){return server;}
protected:
    void run();
private:
    asio::io_service server;
};

#endif // IOSERVERTHREAD_H
