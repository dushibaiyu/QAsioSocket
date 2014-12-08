#include "qasiotcpsocket.h"

QAsioTcpSocket::QAsioTcpSocket(asio::io_service &io_service, const protocol_type &protocol, QObject *parent) :
    QIODevice(parent)
{
}
