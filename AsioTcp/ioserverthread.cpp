#include "ioserverthread.h"
#include <QDebug>

IOServerThread::IOServerThread(QObject *parent) :
    QThread(parent)
{
}

IOServerThread::~IOServerThread()
{

    if (!server.stopped())
        server.stop();
}

void IOServerThread::run()
{
    asio::io_service::work  wk(server);
    wk.get_io_service();//无意义，只是为了取消声明变量而不使用得警告。
    server.run();
    qDebug() << "IOServerThread::run";
}
