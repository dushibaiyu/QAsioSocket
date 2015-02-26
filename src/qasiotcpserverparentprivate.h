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
            acceptor = new boost::asio::ip::tcp::acceptor(iosList[lastState]->getIoServer());
            if (stand_) delete stand_;
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
            socket_ = new boost::asio::ip::tcp::socket(iosList[lastState]->getIoServer());
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
            if (acceptor->is_open()){
                acceptor->close();
            }
        }
    }

    boost::system::error_code error_;
protected:
    //切换新连接采用asio::io_service，采用公平队列，一次轮询
    inline void goForward(){
        if (q->IOSize == 0) {
            lastState = 0;
        } else {
            lastState ++;
            if (lastState == static_cast<int>(iosList.size()))
                lastState = 0;
        } }
    //有新连接的回调
    void appectHandle(const boost::system::error_code & code);
private:
    QAsioTcpServerParent * q;
    boost::asio::ip::tcp::acceptor * acceptor;
    std::vector<IOServerThread *> iosList;
    boost::asio::ip::tcp::socket * socket_;
    boost::asio::io_service::strand * stand_;
    int lastState;
};


#endif // QASIOTCPSERVERPARENTPRIVATE

