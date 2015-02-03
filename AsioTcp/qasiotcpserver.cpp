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
    if (socketV6 != nullptr) delete socketV6;
    if (socketV4 != nullptr) delete socketV4;
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
    type_ = None;
    ip_.clear();
}

bool QAsioTcpServer::linstenV4(const asio::ip::tcp::endpoint &endpoint)
{
    goForward();
    if (apv4 == nullptr)
        apv4 = new asio::ip::tcp::acceptor(iosserverList.at(lastState)->getIOServer());
    asio::error_code code;
    apv4->open(endpoint.protocol());
    apv4->bind(endpoint,code);
    if (code)
    {
        this->error_ = code;
        return false;
    }
    apv4->listen(asio::socket_base::max_connections, code);
    if (code)
    {
        this->error_ = code;
        return false;
    }
    if (socketV4 == nullptr)
        socketV4 = new asio::ip::tcp::socket(iosserverList.at(lastState)->getIOServer());
    apv4->async_accept(*socketV4,std::bind(&QAsioTcpServer::appectHandleV4,this,std::placeholders::_1));
    return true;
}

bool QAsioTcpServer::linstenV6(const asio::ip::tcp::endpoint &endpoint)
{
    goForward();
    if (apv6 == nullptr)
        apv6 = new asio::ip::tcp::acceptor(iosserverList.at(lastState)->getIOServer());
    apv6->open(endpoint.protocol());
    asio::error_code code;
    apv6->bind(endpoint,code);
    if (code)
    {
        this->error_ = code;
        return false;
    }
    apv6->listen(asio::socket_base::max_connections, code);
    if (code)
    {
        this->error_ = code;
        return false;
    }
    if (socketV6 == nullptr)
        socketV6 = new asio::ip::tcp::socket(iosserverList.at(lastState)->getIOServer());
    apv6->async_accept(*socketV6,std::bind(&QAsioTcpServer::appectHandleV6,this,std::placeholders::_1));
    return true;
}

bool QAsioTcpServer::listen(qint16 port, ListenType ltype)
{
    bool tmpbool = false;
    close();
    switch (ltype) {
    case IPV4 :
    {
        asio::ip::tcp::endpoint endpot(asio::ip::tcp::v4(),port);
        tmpbool = linstenV4(endpot);
    }
        break;
    case IPV6 :
    {
        asio::ip::tcp::endpoint endpot(asio::ip::tcp::v6(),port);
        tmpbool = linstenV6(endpot);
    }
        break;
    case Both :
    {
        asio::ip::tcp::endpoint endpot4(asio::ip::tcp::v4(),port);
#ifdef Q_OS_WIN
        asio::ip::tcp::endpoint endpot6(asio::ip::tcp::v6(),port);
#endif
        if (linstenV4(endpot4)
        #ifdef Q_OS_WIN
                && linstenV6(endpot6)
        #endif
                )
            tmpbool = true;
        else {
            this->close();
            tmpbool =  false;
        }
    }
        break;
    default:
        break;
    }
    type_ = ltype;
#ifdef Q_OS_LINUX
    type_ = Both;
#endif
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
            tmpbool = linstenV4(endpot);
            type_ = IPV4;
        } else if (address.is_v6()) {
            tmpbool =  linstenV6(endpot);
            type_ = IPV6;
        }
    }
    if (!tmpbool)
        close();
    return tmpbool;
}

void QAsioTcpServer::appectHandleV4(const asio::error_code &code)
{
    if (!code) {
        goForward();
        QAsioTcpSocket * socket = new QAsioTcpSocket(socketV4);
        socketV4 = new asio::ip::tcp::socket(iosserverList.at(lastState)->getIOServer());
        apv4->async_accept(*socketV4,std::bind(&QAsioTcpServer::appectHandleV4,this,std::placeholders::_1));
        socket->moveToThread(this->thread());
        QCoreApplication::postEvent(this,new QAsioNewEvent(socket),Qt::HighEventPriority);
    } else {
        error_ = code;
    }
}

void QAsioTcpServer::appectHandleV6(const asio::error_code &code)
{
    if (!code) {
        goForward();
        QAsioTcpSocket * socket = new QAsioTcpSocket(socketV6);
        socketV6 = new asio::ip::tcp::socket(iosserverList.at(lastState)->getIOServer());
        apv6->async_accept(*socketV6,std::bind(&QAsioTcpServer::appectHandleV6,this,std::placeholders::_1));
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
        emit newConnection(newcon->getNewSocket());
        e->accept();
    } else {
        QObject::customEvent(e);
    }
}
