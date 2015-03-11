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
    inline bool linstenAp(const asio::ip::tcp::endpoint & endpoint) {
        if (!acceptor) {
            acceptor = new asio::ip::tcp::acceptor(iosserverList[lastState]->getIOServer());
        }
        asio::error_code code;
        acceptor->open(endpoint.protocol());
        acceptor->set_option(asio::ip::tcp::acceptor::reuse_address(false),code);
        acceptor->bind(endpoint,code);
        if (code)
        {
            this->error_ = code;
            return false;
        }
        acceptor->listen(asio::socket_base::max_connections, code);
        if (code)
        {
            this->error_ = code;
            return false;
        }
        if (!socket_)
            socket_ = new asio::ip::tcp::socket(iosserverList[lastState]->getIOServer());
        acceptor->async_accept(*socket_,
                               std::bind(&QAsioTcpServerParentPrivate::appectHandle,this,std::placeholders::_1));
        return true;
    }

    inline bool setNewSocket(QAsioTcpSocketParent * socket)
    {
        bool istrue = (socket->p)->setTcpSocket(socket_);
        if (istrue) {
            socket_ = 0;
        }
        return istrue;
    }

    inline void close(){
        if (acceptor) {
            asio::error_code tcode_;
            acceptor->cancel(tcode_);
            if (acceptor->is_open()){
                acceptor->close(tcode_);
            }
        }
    }

    asio::error_code error_;
protected:
    //有新连接的回调
    void appectHandle(const asio::error_code & code);

    inline void goForward(){lastState ++; if (lastState == static_cast<int>(iosserverList.size())) lastState = 0;}
private:
    QAsioTcpServerParent * q;
    int lastState = 0;
    asio::ip::tcp::acceptor * acceptor;
    asio::ip::tcp::socket * socket_;
    std::vector<IOServerThread *> iosserverList;
};


#endif // QASIOTCPSERVERPARENTPRIVATE

