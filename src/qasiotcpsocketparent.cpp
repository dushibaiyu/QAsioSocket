#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "qasiotcpsocketparentprivate.h"
#include <QDebug>

QAsioTcpSocketParentPrivate::QAsioTcpSocketParentPrivate(QAsioTcpSocketParent *tq, int byteSize) :
    q(tq),socket_(0),resolver_(0),timer(0),stand_(0),byteSize_(byteSize),data_(0)
{
    try {
        data_ = new char[byteSize];
    } catch (std::bad_alloc & /*bad*/) {
        data_ = 0;
        qWarning() << "new Bad the new size is " << byteSize;
    }
}

QAsioTcpSocketParentPrivate::~QAsioTcpSocketParentPrivate()
{
    if (resolver_)
        delete resolver_;
    if (socket_) {
        if (socket_->is_open())
            socket_->close(erro_code);
        delete socket_;
    }
    if (stand_)
        delete stand_;
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
        q->readDataed(data_,bytes_transferred);
        if (timer) {
            timer->cancel();
            timer->async_wait(boost::bind(&QAsioTcpSocketParentPrivate::heartTimeOutHandler,this,boost::asio::placeholders::error));
        }
        socket_->async_read_some(boost::asio::buffer(data_,byteSize_),
                                 stand_->wrap(boost::bind(&QAsioTcpSocketParentPrivate::readHandler,this,
                                             boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred)));

    } else {
        q->state_ = QAsioTcpSocketParent::UnconnectedState;
        q->erro_site = QAsioTcpSocketParent::ReadError;
        if (socket_->is_open()) {
            socket_->close(erro_code);
        }
        erro_code = error;
        if (timer)
            timer->cancel();
        q->haveErro();
    }
}

void QAsioTcpSocketParentPrivate::writeHandler(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (!error && q->state_ == QAsioTcpSocketParent::ConnectedState){
        if (!q->writeDataed(bytes_transferred))
            disconnectFromHost();
    } else {
        q->state_ = QAsioTcpSocketParent::UnconnectedState;
        q->erro_site = QAsioTcpSocketParent::WriteEorro;
        if (socket_->is_open()) {
            socket_->close(erro_code);
        }
        erro_code = error;
        if (timer)
            timer->cancel();
        q->haveErro();
    }
}

void QAsioTcpSocketParentPrivate::resolverHandle(const boost::system::error_code &error, boost::asio::ip::tcp::resolver::iterator iter)
{
    if (!error) {
        boost::asio::async_connect((*socket_),iter,
                    stand_->wrap(boost::bind(&QAsioTcpSocketParentPrivate::connectedHandler,
                                             this,boost::asio::placeholders::error,boost::asio::placeholders::iterator)));
        q->finedHosted();
    } else {
        q->state_ = QAsioTcpSocketParent::UnconnectedState;
        q->erro_site = QAsioTcpSocketParent::FindHostError;
        erro_code = error;
        if (timer)
            timer->cancel();
        q->haveErro();
    }
}

void QAsioTcpSocketParentPrivate::connectedHandler(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator iter)
{
    if (!error) {
        if (data_) {
            q->state_ = QAsioTcpSocketParent::ConnectedState;
            q->erro_site = QAsioTcpSocketParent::NoError;
            socket_->async_read_some(boost::asio::buffer(data_,byteSize_),
                                     stand_->wrap(boost::bind(&QAsioTcpSocketParentPrivate::readHandler,this,
                                                 boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred)));
            boost::asio::ip::tcp::endpoint peerPoint = socket_->remote_endpoint(erro_code);
            q->peerIp = QString::fromStdString(peerPoint.address().to_string());
            q->peerPort = peerPoint.port();
            q->hostConnected();
            if (timer)
                timer->async_wait(boost::bind(&QAsioTcpSocketParentPrivate::heartTimeOutHandler,this,boost::asio::placeholders::error));
        } else {
            q->state_ = QAsioTcpSocketParent::UnconnectedState;
            q->erro_site = QAsioTcpSocketParent::NoBufferSize;
            if (socket_->is_open()) {
                socket_->close(erro_code);
            }
            erro_code = error;
            if (timer)
                timer->cancel();
            q->haveErro();
        }
    } else {
        boost::asio::ip::tcp::resolver::iterator end;
        if(iter == end)
        {
            q->state_ = QAsioTcpSocketParent::UnconnectedState;
            q->erro_site = QAsioTcpSocketParent::ConnectEorro;
            if (socket_->is_open()) {
                socket_->close(erro_code);
            }
            erro_code = error;
            if (timer)
                timer->cancel();
            q->haveErro();
        }
    }
}

void QAsioTcpSocketParentPrivate::heartTimeOutHandler(const boost::system::error_code & error) {
    if (!error) {
        q->heartTimeOut(q->timeOut_s);
    }
    timer->async_wait(boost::bind(&QAsioTcpSocketParentPrivate::heartTimeOutHandler,this,boost::asio::placeholders::error));
}


QAsioTcpSocketParent::QAsioTcpSocketParent(int byteSize, QObject *parent) :
    QObject(parent),erro_site(NoError),peerPort(0),timeOut_s(0)
{
    p = new QAsioTcpSocketParentPrivate(this,byteSize);
    state_ =  UnconnectedState;
}

QAsioTcpSocketParent::~QAsioTcpSocketParent()
{
    delete p;
}

void QAsioTcpSocketParent::connectToHost(const QString & hostName, quint16 port)
{
    p->connectToHost(hostName,port);
    state_ = ConnectingState;
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
//template<typename TcpSocket>
//bool QAsioTcpSocketParent::setTcpSocket(TcpSocket *socket)
//{
//    return p->setTcpSocket(socket);
//}

void QAsioTcpSocketParent::setHeartTimeOut(int s)
{
    if (s < 10) s = 10;
    this->timeOut_s = s;
    p->setHeartTimeOut();
}

void QAsioTcpSocketParent::wirteData(const char * data,std::size_t size)
{
    p->wirteData(data,size);
}
