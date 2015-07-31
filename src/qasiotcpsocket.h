#ifndef QASIOTCPSOCKET_H
#define QASIOTCPSOCKET_H
#include "tcpabstractsocket.h"

struct Session;
class IOServiceClass;

class QASIOSOCKET_EXPORT QAsioTcpsocket : public TcpAbstractSocket
{
    Q_OBJECT
public:
    explicit QAsioTcpsocket(size_t size = 4096,QObject *parent = 0);
    QAsioTcpsocket(Session *ses);
    ~QAsioTcpsocket();

signals:

public slots:
    void connectToHost(const QString &hostName, quint16 port);

    void disconnectFromHost();

    virtual void setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble, uint value);

    virtual std::pair<bool,int> getSocketOption(TcpAbstractSocket::SocketOption opention);

    void do_start();

    int resizeClientBackThreadSize(int size);
protected:
    virtual void hostConnected();
    void sendData(const char * data, int size);
private:
    bool serverBuild;
    Session * con_;
    static IOServiceClass * ioserver;
    static volatile ulong linkCout;
    Q_DISABLE_COPY(QAsioTcpsocket)
};

#endif // QASIOTCPSOCKET_H
