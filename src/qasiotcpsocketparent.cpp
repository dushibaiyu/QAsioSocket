#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "qasiotcpsocketparentprivate.h"
#include <QDebug>

QAsioTcpSocketParentPrivate::QAsioTcpSocketParentPrivate(int byteSize) :
    q(0),socket_(0),resolver_(0),byteSize_(byteSize),data_(0)
{
    try {
        data_ = new char[byteSize];
    } catch (std::bad_alloc & /*bad*/) {
        data_ = 0;
        qWarning() << "new Bad the new size is " << byteSize;
    }
    state_ = QAsioTcpSocketParent::UnconnectedState;
    erro_site = QAsioTcpSocketParent::ReadError;
    peerPort = 0;
}

QAsioTcpSocketParentPrivate::~QAsioTcpSocketParentPrivate()
{
    if (socket_) {
        asio::error_code errocode;
        socket_->close(errocode);
        delete socket_;
    }
    if (resolver_)
        resolver_->cancel();
        delete resolver_;
    if (data_)
        delete[] data_;
}

void QAsioTcpSocketParentPrivate::readHandler(const asio::error_code& error, std::size_t bytes_transferred)
{
    if(!error){
        if (bytes_transferred == 0){
            disconnectFromHost();
            return;
        }
        if (q) {
            q->readDataed(data_,bytes_transferred);
            socket_->async_read_some(asio::buffer(data_,byteSize_),
                                     std::bind(&QAsioTcpSocketParentPrivate::readHandler,shared_from_this(),
                                                              std::placeholders::_1,std::placeholders::_2));
        }

    } else {
        setError(error,QAsioTcpSocketParent::ReadError);
    }
}

void QAsioTcpSocketParentPrivate::writeHandler(const asio::error_code& error, std::size_t bytes_transferred)
{
    if (!error && q){
        if (!q->writeDataed(bytes_transferred))
            disconnectFromHost();
    } else {
        setError(error,QAsioTcpSocketParent::WriteEorro);
    }
}

void QAsioTcpSocketParentPrivate::resolverHandle(const asio::error_code &error, asio::ip::tcp::resolver::iterator iter)
{
    if (!error && q) {
        q->finedHosted();
        if (!socket_) {
            socket_ = new asio::ip::tcp::socket(IOServerThread::getIOThread()->getIOServer());
        }
        asio::async_connect((*socket_),iter,
                    std::bind(&QAsioTcpSocketParentPrivate::connectedHandler,
                                             shared_from_this(),std::placeholders::_1,std::placeholders::_2));
    } else {
        setError(error,QAsioTcpSocketParent::FindHostError);
    }
}

void QAsioTcpSocketParentPrivate::connectedHandler(const asio::error_code& error, asio::ip::tcp::resolver::iterator iter)
{
    if (!error) {
        if (data_){
            state_ = QAsioTcpSocketParent::ConnectedState;
            erro_site = QAsioTcpSocketParent::NoError;
            asio::ip::tcp::endpoint peerPoint = socket_->remote_endpoint(erro_code);
            peerIp = QString::fromStdString(peerPoint.address().to_string());
            peerPort = peerPoint.port();
            if (q) {
                q->hostConnected();
                socket_->async_read_some(asio::buffer(data_,byteSize_),
                                         std::bind(&QAsioTcpSocketParentPrivate::readHandler,shared_from_this(),
                                                     std::placeholders::_1,std::placeholders::_2));
            }
        } else {
            disconnectFromHost();
            setError(error,QAsioTcpSocketParent::NoBufferSize);
        }
    } else {
        asio::ip::tcp::resolver::iterator end;
        if(iter == end)
        {
            setError(error,QAsioTcpSocketParent::ConnectEorro);
        }
    }
}


QAsioTcpSocketParent::QAsioTcpSocketParent(int byteSize, QObject *parent) :
    QObject(parent),timeOut_s(0),p(new QAsioTcpSocketParentPrivate(byteSize))
{
    p->setQPoint(this);
}

QAsioTcpSocketParent::~QAsioTcpSocketParent()
{
    willDelete();
}

void QAsioTcpSocketParent::willDelete()
{
    p->setQPoint(0);
}

void QAsioTcpSocketParent::connectToHost(const QString & hostName, quint16 port)
{
    p->connectToHost(hostName,port);
    emit stateChange(ConnectingState);
}

void QAsioTcpSocketParent::disconnectFromHost()
{
    p->disconnectFromHost();
}

int QAsioTcpSocketParent::socketDescriptor() const
{
    return p->socketDescriptor();
}

int QAsioTcpSocketParent::error() const
{
    return p->erro_code.value();
}

void QAsioTcpSocketParent::setHeartTimeOut(int /*s*/)
{
    this->timeOut_s = 0;
}

void QAsioTcpSocketParent::wirteData(const char * data,std::size_t size)
{
    p->wirteData(data,size);
}

QAsioTcpSocketParent::SocketState QAsioTcpSocketParent::state() const
{
    return p->state_;
}

QAsioTcpSocketParent::SocketErroSite QAsioTcpSocketParent::erroSite() const
{
    return p->erro_site;
}

QString QAsioTcpSocketParent::getPeerIp() const
{
    return p->peerIp;
}

qint16 QAsioTcpSocketParent::getPeerPort() const
{
    return p->peerPort;
}
