#include "ioserverthread.h"

IOServerThread::IOServerThread(QObject *parent) :
    QThread(parent)
{
}

IOServerThread::~IOServerThread()
{
    if (server != nullptr)
    {
        if (!server->stopped())
            server->stop();
        delete server;
    }
}

void IOServerThread::run()
{
    if (server == nullptr)
        server = new asio::io_service;
    asio::io_service::work  wk(*server);
    wk;//无意义，只是为了取消声明变量而不使用得警告。
    server->run();
}
