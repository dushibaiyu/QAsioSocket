#include <QCoreApplication>
#include "QDebug"
#include "qasiotcpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QAsioTcpServer server;
    server.listen(8888,QAsioTcpServer::IPV4);

    qDebug() << "yy";
    return a.exec();
}
