#ifndef QASIOTCPSOCKET_H
#define QASIOTCPSOCKET_H

#include <QIODevice>
#include <QBuffer>
#include "asio.hpp"

class QAsioTcpSocket : public QIODevice,public asio::ip::tcp::socket
{
    Q_OBJECT
public:
    explicit QAsioTcpSocket(asio::io_service& io_service,const protocol_type& protocol, QObject *parent = 0);
    QAsioTcpSocket(asio::io_service& io_service, QObject *parent = 0);
    QAsioTcpSocket(asio::io_service& io_service, const endpoint_type& endpoint, QObject *parent = 0);
    QAsioTcpSocket(asio::io_service& io_service,
                   const protocol_type& protocol, const native_handle_type& native_socket, QObject *parent = 0);

signals:

public slots:

protected:
    void readHandler(const asio::error_code& error, std::size_t bytes_transferred);
    void writeHandler(const asio::error_code& error,std::size_t bytes_transferred);
private:
    QBuffer buffer;
    char data_[2048] = {0};
    qint16 readPos = 0;
//    asio::ip::tcp::socket socket_;
    Q_DISABLE_COPY(QAsioTcpSocket)
};

#endif // QASIOTCPSOCKET_H
