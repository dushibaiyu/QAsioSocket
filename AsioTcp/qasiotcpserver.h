#ifndef QASIOTCPSERVER_H
#define QASIOTCPSERVER_H

#include <QObject>
#include <QVector>
#include "ioserverthread.h"
#include <QMutex>

class QAsioTcpSocket;

class QAsioTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit QAsioTcpServer(int threadSize = -1,QObject *parent = 0);
    ~QAsioTcpServer();
    enum ListenType{
        IPV4,IPV6,Both,Address,None
    };
    asio::error_code getEorror() const {return error_;}
Q_SIGNALS:
    void newConnection(QAsioTcpSocket * socket);
public Q_SLOTS:
    bool listen(const QString & ip,qint16 port);
    bool listen(qint16 port,ListenType ltype = Both);
    void close();
    qint16 listenPort()const {return port_;}
    ListenType listenType() const {return type_;}
    int getIOSize() const {return threadSize_;}
protected:
    void appectHandleV4(const asio::error_code & code);
    void appectHandleV6(const asio::error_code & code);

    void customEvent(QEvent * e);

    inline void goForward(){lastState ++; if (lastState == threadSize_) lastState = 0;}
    inline bool linstenV4(const asio::ip::tcp::endpoint & endpoint);
    inline bool linstenV6(const asio::ip::tcp::endpoint & endpoint);
private:
    int lastState = 0,threadSize_;
    qint16 port_;
    ListenType type_ = None;
    QString ip_;
    asio::error_code error_;
    asio::ip::tcp::acceptor * apv4 = nullptr, * apv6 = nullptr;
    QVector<IOServerThread *> iosserverList;
    asio::ip::tcp::socket * socketV6 = nullptr, * socketV4 = nullptr;
};

#endif // QASIOTCPSERVER_H
