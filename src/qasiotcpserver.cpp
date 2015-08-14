#include "qasiotcpserver.h"
#include "linstenclass.hpp"
#include "qsocketconnection.h"

class TcpLinstenClass;

struct SocketSession {
    IOServiceClass io_;
    size_t size_;
    QAsioTcpServer * server;
    std::shared_ptr<TcpLinstenClass>  * listen_;
    SocketSession(QAsioTcpServer * ser,size_t size = 4096);
    ~SocketSession();
    SocketSession() = delete;
    Session * haveNewAppent(Session * son){
//        qDebug() << "haveNewAppent(Session * son)";
        Session * newCon = new Session(io_.getService(),size_);
        server->incomingConnection(son);
        return newCon;
    }
    void doAppent();
};


class TcpLinstenClass :
        public LinstenClass<Session,SocketSession>,
        public std::enable_shared_from_this<TcpLinstenClass>,
        public asio::noncopyable
{
public :
    TcpLinstenClass(Session * connec, SocketSession * tq,IO_Service &io_s) :
        LinstenClass<Session,SocketSession>(connec,tq,io_s)
    {}
    ~TcpLinstenClass(){}

    inline bool linstenAp(const asio::ip::tcp::endpoint & endpoint){
        if (linsten(endpoint)) {
            do_appent();
            return true;
        } else {
            return false;
        }
    }
    inline void do_appent(){
        acceptor->async_accept(connection->socket(),
                               stand_->wrap(std::bind(&LinstenClass::appectHandle,shared_from_this(),std::placeholders::_1)));
    }

};


SocketSession::SocketSession(QAsioTcpServer * ser,size_t size):size_(size),server(ser) ,
    listen_(new std::shared_ptr<TcpLinstenClass>(new TcpLinstenClass(new Session(io_.getService(),size),this,io_.getService())))
{}

void SocketSession::doAppent()
{
    (*listen_)->do_appent();
}

SocketSession::~SocketSession() {
    (*listen_)->clear();
    delete listen_;
}

QAsioTcpServer::QAsioTcpServer(size_t socket_buffer, QObject *parent) :
    QObject(parent),listen_session(new SocketSession(this,socket_buffer))
{

}

QAsioTcpServer::~QAsioTcpServer()
{
    close();
    delete listen_session;
}

void QAsioTcpServer::incomingConnection(Session * session)
{
    QAsioTcpsocket * socket = new QAsioTcpsocket(session);
    socket->moveToThread(this->thread());
    connect(this,&QAsioTcpServer::deleteAllLink,socket,&QAsioTcpsocket::deleteLater);
    emit newConnection(socket);
}

void QAsioTcpServer::setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble, uint value)
{
    (*listen_session->listen_)->setSocketOption(option,isEnble,value);
}

bool QAsioTcpServer::listen(const QString & ip,qint16 port)
{
    close();
    asio::error_code code;
    asio::ip::address address = asio::ip::address::from_string(ip.toStdString(),code);
    asio::ip::tcp::endpoint endpot(address,port);
    return (*listen_session->listen_)->linstenAp(endpot);
}

bool QAsioTcpServer::listen(qint16 port,bool isV6)
{
    close();
    asio::ip::tcp::endpoint endpot;
    if (isV6)
        endpot = asio::ip::tcp::endpoint(asio::ip::tcp::v6(),port);
    else
        endpot = asio::ip::tcp::endpoint(asio::ip::tcp::v4(),port);
    return (*listen_session->listen_)->linstenAp(endpot);
}

int QAsioTcpServer::setThreadSize(int  size)
{
    return listen_session->io_.setThreadSize(size);
}

void QAsioTcpServer::close()
{
    (*listen_session->listen_)->close();
}
