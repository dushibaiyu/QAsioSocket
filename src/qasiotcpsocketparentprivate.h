#ifndef QASIOTCPSOCKETPARENTPRIVATE
#define QASIOTCPSOCKETPARENTPRIVATE

#include "ioserverthread.h"
#include "../include/qasiotcpsocketparent.h"
#include <functional>
#include <memory>

class QAsioTcpSocketParentPrivate :
        public std::enable_shared_from_this<QAsioTcpSocketParentPrivate>
{
public :
    QAsioTcpSocketParentPrivate(int byteSize);
    ~QAsioTcpSocketParentPrivate();
    inline void setQPoint(QAsioTcpSocketParent * q){this->q = q;}

    inline bool setTcpSocket(asio::ip::tcp::socket *socket) {
        if (!data_ || !q) return false;
        if (socket_) {
            socket_->close(erro_code);
            delete socket_;
        }
        socket_ = socket;
        if (socket_->is_open()) {
            state_ = QAsioTcpSocketParent::ConnectedState;
            asio::ip::tcp::endpoint peerPoint = socket_->remote_endpoint(erro_code);
            peerIp = QString::fromStdString(peerPoint.address().to_string());
            peerPort = peerPoint.port();
            socket_->async_read_some(asio::buffer(data_,byteSize_),
                                     std::bind(&QAsioTcpSocketParentPrivate::readHandler,shared_from_this(),
                                                 std::placeholders::_1,std::placeholders::_2));
        }
        return true;
    }

    inline  void wirteData(const char * data,std::size_t size) {
        if (socketDescriptor() != -1) {
            asio::async_write(*socket_,asio::buffer(data,size),
                          std::bind(&QAsioTcpSocketParentPrivate::writeHandler,shared_from_this(),
                                                   std::placeholders::_1,std::placeholders::_2));
        }
    }

    inline void connectToHost(const QString & hostName, quint16 port) {
        if (!resolver_)
            resolver_ = new asio::ip::tcp::resolver(IOServerThread::getIOThread()->getIOServer());
        else
            resolver_->cancel();
        state_ = QAsioTcpSocketParent::ConnectingState;
        resolver_->async_resolve(asio::ip::tcp::resolver::query(hostName.toStdString(),QString::number(port).toStdString()),
                                 std::bind(&QAsioTcpSocketParentPrivate::resolverHandle,
                                                          shared_from_this(), std::placeholders::_1,std::placeholders::_2));
    }

    inline void disconnectFromHost() {
        if (state_ == QAsioTcpSocketParent::UnconnectedState) return;
        if (socket_)
            socket_->shutdown(asio::ip::tcp::socket::shutdown_both,erro_code);

    }

    inline  int socketDescriptor() const {
        if (!socket_) {
            return -1;
        } else {
            return static_cast<int>(socket_->native_handle());
        }
    }


    asio::error_code erro_code;
    QAsioTcpSocketParent::SocketState state_;
    QAsioTcpSocketParent::SocketErroSite erro_site;
    QString peerIp;
    qint16 peerPort;
protected:
    // 数据读取的回调函数
    void readHandler(const asio::error_code& error, std::size_t bytes_transferred);
    // 数据写入的回调函数
    void writeHandler(const asio::error_code& error,std::size_t bytes_transferred);
    // 链接之后的回调函数
    void connectedHandler(const asio::error_code& error, asio::ip::tcp::resolver::iterator iter);
    // 解析主机后的回调函数
    void resolverHandle(const asio::error_code & error, asio::ip::tcp::resolver::iterator iter);

protected:
    inline void setError(const asio::error_code & erro,QAsioTcpSocketParent::SocketErroSite state){
        state_ = QAsioTcpSocketParent::UnconnectedState;
        erro_site = state;
        peerIp.clear();
        peerPort = 0;
        erro_code = erro;
        if (q) {
            q->haveErro();
        }
        if (socket_)
            socket_->close();
    }

private:
    QAsioTcpSocketParent * q;
    asio::ip::tcp::socket * socket_;
    asio::ip::tcp::resolver * resolver_;
    int byteSize_;
    //接受数据的缓存
    char * data_;
};

#endif // QASIOTCPSOCKETPARENTPRIVATE

