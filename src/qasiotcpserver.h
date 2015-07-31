#ifndef QASIOTCPSERVER_H
#define QASIOTCPSERVER_H

#include <QObject>
#include "qasiotcpsocket.h"

struct SocketSession;
struct Session;

class QASIOSOCKET_EXPORT QAsioTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit QAsioTcpServer(size_t socket_buffer = 4096,QObject *parent = 0);
    ~QAsioTcpServer();

signals:
    void newConnection(QAsioTcpsocket * socket);

    void deleteAllLink();
public :
    bool listen(const QString & ip,qint16 port);
    bool listen(qint16 port,bool isV6 = false);

    void close();

    void setSocketOption(TcpAbstractSocket::SocketOption option, uint value) {
        setSocketOption(option,false,value);
    }
    void setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble) {
        setSocketOption(option,isEnble,0);
    }

    void setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble, uint value);

    int setThreadSize(int  size);
protected:
    virtual void incomingConnection(Session * session);
private:
    SocketSession * listen_session;
    friend struct SocketSession;
};

#endif // QASIOTCPSERVER_H
