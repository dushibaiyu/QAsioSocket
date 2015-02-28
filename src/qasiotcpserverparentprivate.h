#ifndef QASIOTCPSERVERPARENTPRIVATE
#define QASIOTCPSERVERPARENTPRIVATE

#include "qasiotcpsocketparentprivate.h"
#include "../include/qasiotcpserverparent.h"
#include <vector>

class QAsioTcpServerParentPrivate
{
public:
    explicit QAsioTcpServerParentPrivate(QAsioTcpServerParent * tq);
    ~QAsioTcpServerParentPrivate();

    //监听acceptor
    inline bool linstenAp(const boost::asio::ip::tcp::endpoint & endpoint) {
        if (!acceptor) {
            acceptor = new boost::asio::ip::tcp::acceptor(iosService.getIoServer());
            if (!stand_)
                stand_ = new boost::asio::io_service::strand(acceptor->get_io_service());
        }
        boost::system::error_code code;
        acceptor->open(endpoint.protocol());
        acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(false),code);
        acceptor->bind(endpoint,code);
        if (code)
        {
            this->error_ = code;
            return false;
        }
        acceptor->listen(boost::asio::socket_base::max_connections, code);
        if (code)
        {
            this->error_ = code;
            return false;
        }
        if (!socket_)
            socket_ = new boost::asio::ip::tcp::socket(iosService.getIoServer());
        acceptor->async_accept(*socket_,
                               stand_->wrap(boost::bind(&QAsioTcpServerParentPrivate::appectHandle,this,boost::asio::placeholders::error)));
        return true;
    }

    inline bool setNewSocket(QAsioTcpSocketParent * socket)
    {
        bool istrue = socket->p->setTcpSocket(socket_);
        if (istrue) {
            socket_ = 0;
        }
        return istrue;
    }

    inline void close(){
        if (acceptor) {
            boost::system::error_code tcode_;
            acceptor->cancel(tcode_);
            if (acceptor->is_open()){
                acceptor->close(tcode_);
            }
        }
    }

    boost::system::error_code error_;
protected:
    //有新连接的回调
    void appectHandle(const boost::system::error_code & code);
private:
    QAsioTcpServerParent * q;
    boost::asio::ip::tcp::acceptor * acceptor;
    boost::asio::ip::tcp::socket * socket_;
    boost::asio::io_service::strand * stand_;
    IOServerThread  & iosService;
};


#endif // QASIOTCPSERVERPARENTPRIVATE

