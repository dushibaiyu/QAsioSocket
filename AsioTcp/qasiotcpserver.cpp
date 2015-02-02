#ifdef Q_OS_WIN
#include<Mswsock.h>
#endif

#include "qasiotcpserver.h"
#include "qasiotcpsocket.h"
#include <functional>
#include <QCoreApplication>

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
        apv4 = new asio::ip::tcp::acceptor(iosserverList.at(lastState)->getIOServer());
    else {
        if (apv4->is_open()){
            apv4->close();
        }
        delete apv4;
        apv4 = new asio::ip::tcp::acceptor(iosserverList.at(lastState)->getIOServer());
    }
    asio::error_code code;
    apv4->bind(endpoint,code);
    if (code)
    {
        this->error_ = code;
        return false;
    }
    socketV4 = new asio::ip::tcp::socket(iosserverList.at(lastState)->getIOServer());
    apv6->async_accept(*socketV4,std::bind(&QAsioTcpServer::appectHandleV4,this,std::placeholders::_1));
    return true;
}

bool QAsioTcpServer::linstenV6(const asio::ip::tcp::endpoint &endpoint)
{
    goForward();
    if (apv6 == nullptr)
        apv6 = new asio::ip::tcp::acceptor(iosserverList.at(lastState)->getIOServer());
    else {
        if (apv6->is_open()){
            apv6->close();
        }
        delete apv6;
        apv6 = new asio::ip::tcp::acceptor(iosserverList.at(lastState)->getIOServer());
    }
    asio::error_code code;
    apv6->bind(endpoint,code);
    if (code)
    {
        this->error_ = code;
        return false;
    }
    socketV6 = new asio::ip::tcp::socket(iosserverList.at(lastState)->getIOServer());
    apv6->async_accept(*socketV6,std::bind(&QAsioTcpServer::appectHandleV6,this,std::placeholders::_1));
    return true;
}

bool QAsioTcpServer::listen(qint16 port, ListenType ltype)
{
    bool tmpbool = false;
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
        asio::ip::tcp::endpoint endpot4(asio::ip::tcp::v6(),port);
        asio::ip::tcp::endpoint endpot6(asio::ip::tcp::v4(),port);
        if (linstenV4(endpot4) && linstenV6(endpot6))
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

    return tmpbool;
}

bool QAsioTcpServer::listen(const QString &ip, qint16 port)
{
    bool tmpbool = false;
    asio::error_code code;
    asio::ip::address address = asio::ip::address::from_string(ip.toStdString(),code);
    if (code) {
        this->error_ = code;
        tmpbool =  false;
    }
    asio::ip::tcp::endpoint endpot(address,port);
    if (address.is_v4()) {
        tmpbool = linstenV4(endpot);
    } else if (address.is_v6()) {
        tmpbool =  linstenV6(endpot);
    }
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
