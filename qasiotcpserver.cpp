#include "qasiotcpserver.h"
#include "qasiotcpsocket.h"
#include <functional>
#include <QCoreApplication>

class QAsioNewEvent : public QEvent
{
public:
    explicit QAsioNewEvent(QAsioTcpSocket * socket)
        :QEvent(QAsioSocketEventType),socket_(socket){}
    static const QEvent::Type QAsioNewEventType;// = (QEvent::Type)QEvent::registerEventType();

    QAsioTcpSocket * getNewSocket() const {return socket;}
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
        iosserverList.append(thread);
        thread->start();
    }
}

QAsioTcpServer::~QAsioTcpServer()
{
    for (int i = 0; i < threadSize_; ++i)
    {
        auto thread = iosserverList[i];
        delete thread;
    }
}

void QAsioTcpServer::close()
{
    if (apv4 != nullptr)
        apv4->close();
    if (apv6 != nullptr)
        apv6->close();
}

bool QAsioTcpServer::linstenV4(const asio::ip::tcp::endpoint &endpoint)
{
    goForward();
    if (apv4 == nullptr)
        apv4 = new asio::ip::tcp::acceptor(*(iosserverList.at(lastState)->getIOServer()));
    else {
        if (apv4->is_open()){
            apv4->close();
            apv4->open();
        } else {
            apv4->open();
        }
    }
    asio::error_code code;
    apv4->bind(endpoint,code);
    if (code)
    {
        this->error_ = code;
        return false;
    }
    socketV4 = new asio::ip::tcp::socket(*(iosserverList.at(lastState)->getIOServer()));
    apv6->async_accept(*socketV4,std::bind(&QAsioTcpServer::appectHandleV4,this,std::placeholders::_1));
    return true;
}

bool QAsioTcpServer::linstenV6(const asio::ip::tcp::endpoint &endpoint)
{
    goForward();
    if (apv6 == nullptr)
        apv6 = new asio::ip::tcp::acceptor(*(iosserverList.at(lastState)->getIOServer()));
    else {
        if (apv6->is_open()){
            apv6->close();
            apv6->open();
        } else {
            apv6->open();
        }
    }
    asio::error_code code;
    apv6->bind(endpoint,code);
    if (code)
    {
        this->error_ = code;
        return false;
    }
    socketV6 = new asio::ip::tcp::socket(*(iosserverList.at(lastState)->getIOServer()));
    apv6->async_accept(*socketV6,std::bind(&QAsioTcpServer::appectHandleV6,this,std::placeholders::_1));
    return true;
}

bool QAsioTcpServer::listen(qint16 port, ListenType type)
{
    switch (type) {
    case value:

        break;
    default:
        break;
    }
}
