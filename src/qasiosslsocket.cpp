#ifdef QASIO_SSL
#include "qasiosslsocket.h"
#include "qsocketconnection.h"

QAsioSslSocket::QAsioSslSocket(SSLSession *session):
    TcpAbstractSocket(0),con_(session)
{
    int soc = con_->connection->socketDescriptor();
    if (soc != -1) {
        setState(ConnectedState);
        setSocketPtr(soc);
    }
    con_->connection->setQTcp(this);
}

QAsioSslSocket::~QAsioSslSocket()
{
    con_->connection->setQTcp(nullptr);
    delete con_;
}

void QAsioSslSocket::do_start()
{
   con_->connection->do_read();
}


void QAsioSslSocket::disconnectFromHost()
{
    if (state() == UnconnectedState) return;
        con_->connection->disconnectFromHost();
}

void QAsioSslSocket::sendData(const char * data, int size)
{
    if (state() == ConnectedState)
        con_->connection->wirteData(data,size);
}

void QAsioSslSocket::setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble, uint value)
{
    con_->connection->setSocketOption(option,isEnble,value);
}

std::pair<bool,int> QAsioSslSocket::getSocketOption(TcpAbstractSocket::SocketOption opention)
{
    return con_->connection->getSocketOption(opention);
}

#endif
