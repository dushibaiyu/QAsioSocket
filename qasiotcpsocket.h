#ifndef QASIOTCPSOCKET_H
#define QASIOTCPSOCKET_H

#include <QIODevice>
#include <QBuffer>
#include "asio.hpp"

class QAsioTcpSocket : public QIODevice//,public asio::ip::tcp::socket
{
    Q_OBJECT
public:
    explicit QAsioTcpSocket(asio::ip::tcp::socket * socket , QObject *parent = 0);

signals:

public:
    qint64 write_sync(const QByteArray & data);
    qint64 write_sync(const char * data, qint64 maxSize);
    qint64 write_sync(const char * data);
protected:
    void readHandler(const asio::error_code& error, std::size_t bytes_transferred);
    void writeHandler(const asio::error_code& error,std::size_t bytes_transferred);
    void readData(char *data, qint64 maxlen);
    void readLine(char *data, qint64 maxlen);
    qint64 writeData(const char * data, qint64 maxSize);
private:
    QBuffer buffer;
    char data_[2048] = {0};
    qint16 readPos = 0;
    asio::ip::tcp::socket * socket_;
    Q_DISABLE_COPY(QAsioTcpSocket)
};

#endif // QASIOTCPSOCKET_H
