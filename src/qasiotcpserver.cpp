#ifdef Q_OS_WIN
#include<Mswsock.h>
#endif

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif


#include "qasiotcpserver.h"
#include "qasiotcpsocket.h"
#include <functional>
#include <QCoreApplication>

//asio事件循环线程与server主线程有新连接交互的事件
class QAsioNewEvent : public QEvent
{
public:
    explicit QAsioNewEvent(QAsioTcpSocket * socket)
        :QEvent(QAsioNewEventType),socket_(socket){}
    static const QEvent::Type QAsioNewEventType;// = (QEvent::Type)QEvent::registerEventType();

    QAsioTcpSocket * getNewSocket() const {return socket_;}
private:
    QAsioTcpSocket * socket_;
};

const QEvent::Type QAsioNewEvent::QAsioNewEventType = (QEvent::Type)QEvent::registerEventType();

QAsioTcpServer::QAsioTcpServer(int threadSize, QObject *parent) :
    QObject(parent),threadSize_(threadSize)
{
    if (threadSize_ <= 0) threadSize_ = 2;
    for (int i = 0; i < threadSize_; ++i)
    {
        auto thread = new IOServerThread(this);
        thread->start();
        iosserverList.append(thread);
    }
}

QAsioTcpServer::~QAsioTcpServer()
{
    close();
    if (apv4 != nullptr) delete apv4;
    if (apv6 != nullptr) delete apv6;
    if (apall != nullptr) delete apall;
    if (socket_ != nullptr) delete socket_;
    for (int i = 0; i < threadSize_; ++i)
    {
        auto thread = iosserverList[i];
        delete thread;
    }
}

void QAsioTcpServer::close()
{
    if (apv4 != nullptr) {
        if (apv4->is_open()){
            apv4->close();
        }
    }
    if (apv6 != nullptr) {
        if (apv6->is_open()){
            apv6->close();
        }
    }
    if (apall != nullptr) {
        if (apall->is_open()){
            apall->close();
        }
    }
    type_ = None;
    ip_.clear();
}

bool QAsioTcpServer::linstenAp(asio::ip::tcp::acceptor * ap,const asio::ip::tcp::endpoint & endpoint)
{
    goForward();
    asio::error_code code;
    ap->open(endpoint.protocol());
    ap->set_option(asio::ip::tcp::acceptor::reuse_address(false),code);
    ap->bind(endpoint,code);
    if (code)
    {
        this->error_ = code;
        return false;
    }
    ap->listen(asio::socket_base::max_connections, code);
    if (code)
    {
        this->error_ = code;
        return false;
    }
    if (socket_ == nullptr)
        socket_ = new asio::ip::tcp::socket(iosserverList.at(lastState)->getIOServer());
    ap->async_accept(*socket_,std::bind(&QAsioTcpServer::appectHandle,this,std::placeholders::_1));
    return true;
}

bool QAsioTcpServer::listen(qint16 port, ListenType ltype)
{
    bool tmpbool = false;
    close();
#ifdef Q_OS_LINUX
    ltype = Both;
#endif
    switch (ltype) {
    case IPV4 :
    {
        asio::ip::tcp::endpoint endpot(asio::ip::tcp::v4(),port);
        if (apv4 == nullptr)
            apv4 = new asio::ip::tcp::acceptor(iosserverList.at(lastState)->getIOServer());
        tmpbool = linstenAp(apv4,endpot);
    }
        break;
    case IPV6 :
    {
        asio::ip::tcp::endpoint endpot(asio::ip::tcp::v6(),port);
        if (apv6 == nullptr)
            apv6 = new asio::ip::tcp::acceptor(iosserverList.at(lastState)->getIOServer());
        tmpbool = linstenAp(apv6,endpot);
    }
        break;
    case Both :
    {
        asio::ip::tcp::endpoint endpot(asio::ip::tcp::all(),port);
        if (apall == nullptr)
            apall = new asio::ip::tcp::acceptor(iosserverList.at(lastState)->getIOServer());
        tmpbool = linstenAp(apall,endpot);
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

bool QAsioTcpServer::listen(const QString &ip, qint16 port)
{
    bool tmpbool = false;
    close();
    asio::error_code code;
    asio::ip::address address = asio::ip::address::from_string(ip.toStdString(),code);
    if (code) {
        this->error_ = code;
        tmpbool =  false;
    } else {
        ip_ = ip;
        asio::ip::tcp::endpoint endpot(address,port);
        if (address.is_v4()) {
            if (apv4 == nullptr)
                apv4 = new asio::ip::tcp::acceptor(iosserverList.at(lastState)->getIOServer());
            tmpbool = linstenAp(apv4,endpot);
            type_ = IPV4;
        } else if (address.is_v6()) {
            if (apv6 == nullptr)
                apv6 = new asio::ip::tcp::acceptor(iosserverList.at(lastState)->getIOServer());
            tmpbool = linstenAp(apv6,endpot);
            type_ = IPV6;
        }
    }
    if (!tmpbool)
        close();
    return tmpbool;
}

void QAsioTcpServer::appectHandle(const asio::error_code &code)
{
    if (!code) {
        goForward();
        QAsioTcpSocket * socket = new QAsioTcpSocket(socket_);
        socket_ = new asio::ip::tcp::socket(iosserverList.at(lastState)->getIOServer());
        switch (type_) {
        case IPV4 :
            apv4->async_accept(*socket_,std::bind(&QAsioTcpServer::appectHandle,this,std::placeholders::_1));
            break;
        case IPV6 :
            apv6->async_accept(*socket_,std::bind(&QAsioTcpServer::appectHandle,this,std::placeholders::_1));
            break;
        case Both :
            apall->async_accept(*socket_,std::bind(&QAsioTcpServer::appectHandle,this,std::placeholders::_1));
            break;
        default:
            break;
        }
        socket->moveToThread(this->thread());
        QCoreApplication::postEvent(this,new QAsioNewEvent(socket),Qt::HighEventPriority);
    } else {
        error_ = code;
    }
}

void QAsioTcpServer::customEvent(QEvent *e)
{
    if (e->type() == QAsioNewEvent::QAsioNewEventType)
    {
        QAsioNewEvent * newcon = static_cast<QAsioNewEvent *>(e);
        incomingConnection(newcon->getNewSocket());
        e->accept();
    } else {
        QObject::customEvent(e);
    }
}

void QAsioTcpServer::incomingConnection(QAsioTcpSocket * socket)
{
    socket->setDisconnecdDeleteBuffer(true);
    emit newConnection(socket);
}
