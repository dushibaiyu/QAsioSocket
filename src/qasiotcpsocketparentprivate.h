#ifndef QASIOTCPSOCKETPARENTPRIVATE
#define QASIOTCPSOCKETPARENTPRIVATE

#include "ioserverthread.h"
#include "../include/qasiotcpsocketparent.h"
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

class QAsioTcpSocketParentPrivate :
        public boost::enable_shared_from_this<QAsioTcpSocketParentPrivate>,
        private boost::noncopyable
{
public :
    QAsioTcpSocketParentPrivate(int byteSize);
    ~QAsioTcpSocketParentPrivate();
    inline void setQPoint(QAsioTcpSocketParent * q){this->q = q;}
    inline bool setTcpSocket(boost::asio::ip::tcp::socket *socket) {
        if (!data_ || !q) return false;
        if (socket_) {
            socket_->close(erro_code);
            delete socket_;
        }
        socket_ = socket;
        if (!stand_)
            stand_ = new boost::asio::io_service::strand(IOServerThread::getIOThread().getIoServer());
        setHeartTimeOut();
        if (socket_->is_open()) {
            boost::asio::ip::tcp::endpoint peerPoint = socket_->remote_endpoint(erro_code);
            q->peerIp = QString::fromStdString(peerPoint.address().to_string());
            q->peerPort = peerPoint.port();
            socket_->async_read_some(boost::asio::buffer(data_,byteSize_),
                                     stand_->wrap(boost::bind(&QAsioTcpSocketParentPrivate::readHandler,shared_from_this(),
                                                 boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred)));
            q->state_ = QAsioTcpSocketParent::ConnectedState;
            if (timer) {
                timer->cancel();
                timer->async_wait(boost::bind(&QAsioTcpSocketParentPrivate::heartTimeOutHandler,shared_from_this(),boost::asio::placeholders::error));
            }
        }
        return true;
    }
    inline  void wirteData(const char * data,std::size_t size) {
        if (socketDescriptor() != -1) {
            boost::asio::async_write(*socket_,boost::asio::buffer(data,size),
                          stand_->wrap(boost::bind(&QAsioTcpSocketParentPrivate::writeHandler,shared_from_this(),
                                                   boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred)));
        }
    }
    inline void connectToHost(const QString & hostName, quint16 port) {
        if (!resolver_)
            resolver_ = new boost::asio::ip::tcp::resolver(IOServerThread::getIOThread().getIoServer());
        else
            resolver_->cancel();
        if (!stand_)
            stand_ = new boost::asio::io_service::strand(IOServerThread::getIOThread().getIoServer());
        if (timer)
            timer->cancel();
        resolver_->async_resolve(boost::asio::ip::tcp::resolver::query(hostName.toStdString(),QString::number(port).toStdString()),
                                 stand_->wrap(boost::bind(&QAsioTcpSocketParentPrivate::resolverHandle,
                                                          shared_from_this(), boost::asio::placeholders::error,boost::asio::placeholders::iterator)));
    }

    inline void disconnectFromHost() {
        if (timer)
            timer->cancel();
        if (q->state_ == QAsioTcpSocketParent::UnconnectedState) return;
        if (socket_)
            socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both,erro_code);
    }

    inline  int socketDescriptor() const {
        if (!socket_) {
            return -1;
        } else {
            return static_cast<int>(socket_->native_handle());
        }
    }

    inline void setHeartTimeOut() {
            if (timer) {
                timer->cancel();
                delete timer;
                timer = 0;
            }
            if (q && !q->timeOut_s){
                return;
            }
            if (socket_) {
                timer = new boost::asio::deadline_timer(IOServerThread::getIOThread().getIoServer(),boost::posix_time::seconds(q->timeOut_s));
                if (q && q->state_ == QAsioTcpSocketParent::ConnectedState)
                  timer->async_wait(boost::bind(&QAsioTcpSocketParentPrivate::heartTimeOutHandler,
                                                shared_from_this(),boost::asio::placeholders::error));
            }
        }

    boost::system::error_code erro_code;
protected:
    // 数据读取的回调函数
    void readHandler(const boost::system::error_code& error, std::size_t bytes_transferred);
    // 数据写入的回调函数
    void writeHandler(const boost::system::error_code& error,std::size_t bytes_transferred);
    // 链接之后的回调函数
    void connectedHandler(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator iter);
    // 解析主机后的回调函数
    void resolverHandle(const boost::system::error_code & error, boost::asio::ip::tcp::resolver::iterator iter);

    void heartTimeOutHandler(const boost::system::error_code& error);

private:
    QAsioTcpSocketParent * q;
    boost::asio::ip::tcp::socket * socket_;
    boost::asio::ip::tcp::resolver * resolver_;
    boost::asio::deadline_timer * timer;
    boost::asio::io_service::strand * stand_;
    int byteSize_;
    //接受数据的缓存
    char * data_;
};

#endif // QASIOTCPSOCKETPARENTPRIVATE

