#include <QCoreApplication>
#include <QDebug>
#include "qasiotcpsocket.h"
#include "testserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    TestServer server;
    server.listen();

    qDebug() << "yy";

    int i = a.exec();
    qDebug() << "end";
    return i;
}
