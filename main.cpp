#include <QCoreApplication>
#include "QDebug"
#include "qasiotcpserver.h"
#include "qasiotcpsocket.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QAsioTcpSocket sock;
    sock.connectToHost("127.0.0.1",6666);
    QObject::connect(&sock,&QAsioTcpSocket::connected,[&](){
        qDebug() << "socketconned";
        sock.write(QByteArray("hahaha"));});
    QObject::connect(&sock,&QAsioTcpSocket::disconnected,[](){qDebug() << "disconnected";});
    QObject::connect(&sock,&QAsioTcpSocket::readReadly,[&](){qDebug() << "readReadly:" << sock.readAll();});
    QObject::connect(&sock,&QAsioTcpSocket::sentError,[&](QAsioTcpSocket::SocketErroSite site,const asio::error_code & /*erro_code*/){
        qDebug() << "erro:" << site;
    });
    qDebug() << "yy";

    int i = a.exec();
    qDebug() << "end";
    return i;
}
