#ifndef QASIOTCPSOCKET_H
#define QASIOTCPSOCKET_H

#include <QBuffer>
#include "ioserverthread.h"
#include <QSharedPointer>
#include <QMutex>
#include <QQueue>

class QAsioTcpSocket : public QObject//,public asio::ip::tcp::socket
{
    Q_OBJECT
public:
    explicit QAsioTcpSocket(QObject *parent = 0);

    enum SocketState {
        UnconnectedState,
        ConnectingState,
        ConnectedState
    };
    enum SocketErroSite{
        ConnectEorro,
        WriteEorro,
        ReadError
    };

signals:
    void readReadly();
    void connected();
    void disconnected();
    void error(SocketErroSite site,asio::error_code & erro_code);
    void stateChange(SocketState state);
    void hostFound();
public slots:
    void connectToHost(const QString &hostName, quint16 port);
    void connectToHost(asio::ip::tcp::endpoint & peerPoint);

    void disconnectFromHost();

    qint64 bytesAvailable() const {return buffer.size() - buffer.pos();}
    QByteArray read(qint64 maxsize = 0)
    {
        QMutexLocker mutexLocker(bufferMutex);
        mutexLocker;
        return buffer.read(maxsize);
    }
    QByteArray readAll()
    {
        QMutexLocker mutexLocker(bufferMutex);
        mutexLocker;
        return buffer.readAll();
    }
    QByteArray readLine()
    {
        QMutexLocker mutexLocker(bufferMutex);
        mutexLocker;
        return buffer.readLine();
    }
    bool atEnd() const{return buffer.atEnd();}

    bool write(const QByteArray & data = QByteArray());

    asio::ip::tcp::endpoint peerEndPoint() const {return this->peerPoint;}
    SocketState state() const {return this->state_;}
    asio::error_code error() const {return this->erro_code;}
protected:
    void readHandler(const asio::error_code& error, std::size_t bytes_transferred);
    void writeHandler(const asio::error_code& error,std::size_t bytes_transferred);
    void connectedHandler(const asio::error_code& error);

    QAsioTcpSocket(asio::ip::tcp::socket * socket , QObject *parent = 0);//server类才能访问

protected:
    void customEvent(QEvent * event);
    static QSharedPointer<IOServerThread> ioserver;
private:
    QMutex writeMutex;
    QQueue<QByteArray> writeQueue;
private:
    QBuffer buffer;
    std::string data_;
    QMutex bufferMutex;
private:
    SocketState state_;
    asio::ip::tcp::socket * socket_;
    asio::error_code erro_code;
    asio::ip::tcp::endpoint peerPoint;
    Q_DISABLE_COPY(QAsioTcpSocket)
};

#endif // QASIOTCPSOCKET_H
