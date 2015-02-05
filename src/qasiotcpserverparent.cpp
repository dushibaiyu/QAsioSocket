#include "qasiotcpserverparent.h"
#include <functional>

QAsioTcpServerParent::QAsioTcpServerParent(int threadSize, QObject *parent)
    : QObject(parent),threadSize_(threadSize),lastState(0)
{
    if (threadSize_ <= 0) threadSize_ = 2;
    for (int i = 0; i < threadSize_; ++i)
    {
        auto thread = new IOServerThread(this);
        thread->start();
        iosserverList.append(thread);
    }
}

QAsioTcpServerParent::~QAsioTcpServerParent()
{
    close();
    if (acceptor != nullptr) delete acceptor;
    if (socket_ != nullptr) delete socket_;
    for (int i = 0; i < threadSize_; ++i)
    {
        auto thread = iosserverList[i];
        delete thread;
    }
}

void QAsioTcpServerParent::close()
{
    if (acceptor != nullptr) {
        if (acceptor->is_open()){
            acceptor->close();
        }
    }
    type_ = None;
    ip_.clear();
}

bool QAsioTcpServerParent::linstenAp(const asio::ip::tcp::endpoint & endpoint)
{
    goForward();
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
    if (socket_ == nullptr)
        socket_ = new asio::ip::tcp::socket(iosserverList.at(lastState)->getIOServer());
    acceptor->async_accept(*socket_,std::bind(&QAsioTcpServerParent::appectHandle,this,std::placeholders::_1));
    return true;
}

bool QAsioTcpServerParent::listen(qint16 port, ListenType ltype)
{
    bool tmpbool = false;
    close();
#ifdef Q_OS_LINUX
    ltype = Both;
#endif
    if (acceptor == nullptr)
        acceptor = new asio::ip::tcp::acceptor(iosserverList.at(lastState)->getIOServer());
    switch (ltype) {
    case IPV4 :
    {
        asio::ip::tcp::endpoint endpot(asio::ip::tcp::v4(),port);
        tmpbool = linstenAp(endpot);
    }
        break;
    case IPV6 :
    {
        asio::ip::tcp::endpoint endpot(asio::ip::tcp::v6(),port);
        tmpbool = linstenAp(endpot);
    }
        break;
    case Both :
    {
        asio::ip::tcp::endpoint endpot(asio::ip::tcp::all(),port);
        tmpbool = linstenAp(endpot);
    }
        break;
    default:
        break;
    }
    type_ = ltype;
    ip_ = "0";
    if (!tmpbool)
        close();
    return tmpbool;
}

bool QAsioTcpServerParent::listen(const QString &ip, qint16 port)
{
    bool tmpbool = false;
    close();
    asio::error_code code;
    asio::ip::address address = asio::ip::address::from_string(ip.toStdString(),code);
    if (code) {
        this->error_ = code;
        tmpbool =  false;
    } else {
        if (acceptor == nullptr)
            acceptor = new asio::ip::tcp::acceptor(iosserverList.at(lastState)->getIOServer());
        ip_ = ip;
        asio::ip::tcp::endpoint endpot(address,port);
        tmpbool = linstenAp(endpot);
    }
    if (!tmpbool)
        close();
    return tmpbool;
}

void QAsioTcpServerParent::appectHandle(const asio::error_code &code)
{
    if (!code) {
        newConnected(socket_);
    } else {
        error_ = code;
        if (haveErro(code)){
            type_ = None;
        }
    }
    if (type_ == None){
        close();
        return;
    }
    goForward();
    socket_ = new asio::ip::tcp::socket(iosserverList.at(lastState)->getIOServer());
    acceptor->async_accept(*socket_,std::bind(&QAsioTcpServerParent::appectHandle,this,std::placeholders::_1));
}


