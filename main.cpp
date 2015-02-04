#include <QCoreApplication>
#include <QDebug>
//#include "qasiotcpsocket.h"
#include "testserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    QAsioTcpSocket sock;
//    sock.connectToHost("127.0.0.1",6666);
//    QObject::connect(&sock,&QAsioTcpSocket::connected,[&](){
//        qDebug() << "socketconned";
//        sock.write(QByteArray("hahaha"));});
//    QObject::connect(&sock,&QAsioTcpSocket::disconnected,[](){qDebug() << "disconnected";});
//    QObject::connect(&sock,&QAsioTcpSocket::readReadly,[&](){qDebug() << "readReadly:" << sock.readAll();sock.disconnectFromHost();});
//    QObject::connect(&sock,&QAsioTcpSocket::sentError,[&](const QString & site,const asio::error_code & /*erro_code*/){
//        qDebug() << "erro:" << site;
//    });
    TestServer server;
    server.listen();

    qDebug() << "yy";

    int i = a.exec();
    qDebug() << "end";
    return i;
}
