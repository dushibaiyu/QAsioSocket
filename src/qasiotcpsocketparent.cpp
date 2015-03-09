#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "qasiotcpsocketparentprivate.h"
#include <QDebug>

QAsioTcpSocketParentPrivate::QAsioTcpSocketParentPrivate(int byteSize) :
    q(0),socket_(0),resolver_(0),timer(0),stand_(0),byteSize_(byteSize),data_(0)
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
    timeOut_s = 0;
}

QAsioTcpSocketParentPrivate::~QAsioTcpSocketParentPrivate()
{
    if (socket_) {
        boost::system::error_code errocode;
        socket_->close(errocode);
        delete socket_;
    }
    if (stand_)
        delete stand_;
    if (resolver_)
        resolver_->cancel();
        delete resolver_;
    if (data_)
        delete[] data_;
}

void QAsioTcpSocketParentPrivate::readHandler(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if(!error){
        if (bytes_transferred == 0){
            disconnectFromHost();
            return;
        }
        if (q) {
            q->readDataed(data_,bytes_transferred);
            socket_->async_read_some(boost::asio::buffer(data_,byteSize_),
                                     stand_->wrap(boost::bind(&QAsioTcpSocketParentPrivate::readHandler,shared_from_this(),
                                                              boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred)));
            if (timer && (timeOut_s > 0 )) {
                timer->expires_from_now(boost::posix_time::seconds(timeOut_s));
            }
        }

    } else {
        setError(error,QAsioTcpSocketParent::ReadError);
    }
}

void QAsioTcpSocketParentPrivate::writeHandler(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (!error && q){
        if (!q->writeDataed(bytes_transferred))
            disconnectFromHost();
    } else {
        setError(error,QAsioTcpSocketParent::WriteEorro);
    }
}

void QAsioTcpSocketParentPrivate::resolverHandle(const boost::system::error_code &error, boost::asio::ip::tcp::resolver::iterator iter)
{
    if (!error && q) {
        q->finedHosted();
        if (!socket_) {
            socket_ = new boost::asio::ip::tcp::socket(IOServerThread::getIOThread().getIoServer());
        }
        boost::asio::async_connect((*socket_),iter,
                    stand_->wrap(boost::bind(&QAsioTcpSocketParentPrivate::connectedHandler,
                                             shared_from_this(),boost::asio::placeholders::error,boost::asio::placeholders::iterator)));
    } else {
        setError(error,QAsioTcpSocketParent::FindHostError);
    }
}

void QAsioTcpSocketParentPrivate::connectedHandler(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator iter)
{
    if (!error) {
        if (data_){
            state_ = QAsioTcpSocketParent::ConnectedState;
            erro_site = QAsioTcpSocketParent::NoError;
            boost::asio::ip::tcp::endpoint peerPoint = socket_->remote_endpoint(erro_code);
            peerIp = QString::fromStdString(peerPoint.address().to_string());
            peerPort = peerPoint.port();
            if (q) {
                q->hostConnected();
                socket_->async_read_some(boost::asio::buffer(data_,byteSize_),
                                         stand_->wrap(boost::bind(&QAsioTcpSocketParentPrivate::readHandler,shared_from_this(),
                                                     boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred)));
                setHeartTimeOut();
            }
        } else {
            disconnectFromHost();
            setError(error,QAsioTcpSocketParent::NoBufferSize);
        }
    } else {
        boost::asio::ip::tcp::resolver::iterator end;
        if(iter == end)
        {
            setError(error,QAsioTcpSocketParent::ConnectEorro);
        }
    }
}

void QAsioTcpSocketParentPrivate::heartTimeOutHandler(const boost::system::error_code & error) {
    if (!error && (state_ == QAsioTcpSocketParent::ConnectedState) && (timeOut_s > 0) && q) {
        q->heartTimeOut(timeOut_s);
        timer->expires_from_now(boost::posix_time::seconds(timeOut_s));
    } else {
        timer->expires_from_now(boost::posix_time::pos_infin);
    }
    timer->async_wait(boost::bind(&QAsioTcpSocketParentPrivate::heartTimeOutHandler,
                                  shared_from_this(),boost::asio::placeholders::error));
}


QAsioTcpSocketParent::QAsioTcpSocketParent(int byteSize, QObject *parent) :
    QObject(parent)
{
    QAsioTcpSocketParentPrivate * tp = new QAsioTcpSocketParentPrivate(byteSize);
    tp->setQPoint(this);
    p = new boost::shared_ptr<QAsioTcpSocketParentPrivate>(tp);
}

QAsioTcpSocketParent::~QAsioTcpSocketParent()
{
    (*p)->setQPoint(0);
    delete p;
}

void QAsioTcpSocketParent::connectToHost(const QString & hostName, quint16 port)
{
    (*p)->connectToHost(hostName,port);
    emit stateChange(ConnectingState);
}

void QAsioTcpSocketParent::disconnectFromHost()
{
    (*p)->disconnectFromHost();
}

int QAsioTcpSocketParent::socketDescriptor() const
{
    return (*p)->socketDescriptor();
}

int QAsioTcpSocketParent::error() const
{
    return (*p)->erro_code.value();
}

void QAsioTcpSocketParent::setHeartTimeOut(int s)
{
    if (s <= 0) (*p)->timeOut_s = 0;
    else if (s < 10) (*p)->timeOut_s = 10;
    (*p)->setHeartTimeOut();
}

void QAsioTcpSocketParent::wirteData(const char * data,std::size_t size)
{
    (*p)->wirteData(data,size);
}

QAsioTcpSocketParent::SocketState QAsioTcpSocketParent::state() const
{
    return (*p)->state_;
}

QAsioTcpSocketParent::SocketErroSite QAsioTcpSocketParent::erroSite() const
{
    return (*p)->erro_site;
}

QString QAsioTcpSocketParent::getPeerIp() const
{
    return (*p)->peerIp;
}

qint16 QAsioTcpSocketParent::getPeerPort() const
{
    return (*p)->peerPort;
}

int QAsioTcpSocketParent::getHeartTimeOut() const
{
    return (*p)->timeOut_s;
}

void QAsioTcpSocketParent::willDelete()
{
    (*p)->setQPoint(0);
}
