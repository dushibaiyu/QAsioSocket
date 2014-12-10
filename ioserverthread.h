#ifndef IOSERVERTHREAD_H
#define IOSERVERTHREAD_H

#include "include/asio.hpp"
#include <QThread>
//#include "asio/io_service.hpp"


class IOServerThread : public QThread
{
    Q_OBJECT
public:
    explicit IOServerThread(QObject *parent = 0);
    ~IOServerThread();

public:
    asio::io_service * getIOServer(){return server;}
protected:
    void run();
private:
    asio::io_service * server = nullptr;
};

#endif // IOSERVERTHREAD_H
