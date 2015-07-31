#ifndef QASIOSSLSOCKET_H
#define QASIOSSLSOCKET_H

#include "tcpabstractsocket.h"
#ifdef QASIO_SSL

class SSLSession;

class QASIOSOCKET_EXPORT QAsioSslSocket : public TcpAbstractSocket
{
    Q_OBJECT
public:
    QAsioSslSocket(SSLSession * session);
    virtual ~QAsioSslSocket();
public slots:
    void disconnectFromHost();

    void setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble, uint value);

    std::pair<bool,int> getSocketOption(TcpAbstractSocket::SocketOption);

    void do_start();
protected slots:
    void sendData(const char * data, int size);
    void connectToHost(const QString &/*hostName*/, quint16 /*port*/)
    {}
private:
    SSLSession * con_;
    Q_DISABLE_COPY(QAsioSslSocket)
};

#endif //asio_ssl

#endif // QASIOSSLSOCKET_H
