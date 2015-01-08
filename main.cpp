#include <QCoreApplication>
#include "QDebug"
#include "qasiotcpserver.h"
#include "qasiotcpsocket.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    QAsioTcpServer server;
//    QObject::connect(&server,&QAsioTcpServer::newConnection,[](QAsioTcpSocket * socket){
//        QObject::connect(socket,&QAsioTcpSocket::readReadly,[&](){
//            socket->write(socket->readAll());
//        });
//        QObject::connect(socket,&QAsioTcpSocket::disconnected,socket,&QAsioTcpSocket::deleteLater);
//    });
//    qDebug() << server.listen(8888,QAsioTcpServer::IPV4);



    qDebug() << "yy";
    int i = a.exec();
    qDebug() << "end";
    return i;
}
