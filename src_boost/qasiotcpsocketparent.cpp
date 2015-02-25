
#include "ioserverthread.h"
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/system/error_code.hpp>
#include <functional>
#include <QDebug>
#include "qasiotcpsocketparent.h"

class QAsioTcpSocketParentPrivate
{
public :
    QAsioTcpSocketParentPrivate(QAsioTcpSocketParent * q,int byteSize);
    ~QAsioTcpSocketParentPrivate();
    inline bool setTcpSocket(boost::asio::ip::tcp::socket *socket) {
        if (!data_) return false;
        if (socket_) {
            if (socket_->is_open())
                socket_->close(erro_code);
            delete socket_;
        }
        socket_ = socket;
        if (stand_) {
            delete stand_;
        }
        stand_ = new boost::asio::io_service::strand(socket_->get_io_service());
        if (q->timeOut_s > 0) {
            setHeartTimeOut();
        }
        if (socket_->is_open()) {
            socket_->async_read_some(boost::asio::buffer(data_,byteSize_),
                                     stand_->wrap(boost::bind(&QAsioTcpSocketParentPrivate::readHandler,this,
                                                 boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred)));
            q->state_ = QAsioTcpSocketParent::ConnectedState;
            if (timer) {
                timer->cancel();
                timer->async_wait(boost::bind(&QAsioTcpSocketParentPrivate::heartTimeOutHandler,boost::asio::placeholders::error));
            }
        }
        return true;
    }
    inline  void wirteData(const char * data,std::size_t size) {
        boost::asio::async_write(*socket_,boost::asio::buffer(data,size),
                          stand_->wrap(boost::bind(&QAsioTcpSocketParentPrivate::writeHandler,this,
                                                   boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred)));
    }
    /*inline */void connectToHost(const QString & hostName, quint16 port);
//{
//        if (!socket_){
//            socket_ = new boost::asio::ip::tcp::socket(IOServerThread::getIOThread().getIoServer());
//            if (stand_) {
//                delete stand_;
//            }
//            stand_ = new boost::asio::io_service::strand(socket_->get_io_service());
//            if (q->timeOut_s > 0) {
//                setHeartTimeOut();
//            }
//        } else if (socket_->is_open()) {
//            socket_->close(erro_code);
//        }
//        if (!resolver_)
//            resolver_ = new boost::asio::ip::tcp::resolver(socket_->get_io_service());
//        if (timer)
//            timer->cancel();
//        resolver_->async_resolve(boost::asio::ip::tcp::resolver::query(hostName.toStdString(),QString::number(port).toStdString()),
//                                 boost::bind(&QAsioTcpSocketParentPrivate::resolverHandle,this, _1,_2));
//    }

    inline void disconnectFromHost() {
        if (timer)
            timer->cancel();
        if (q->state_ == QAsioTcpSocketParent::UnconnectedState) return;
        socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    }

    inline  int socketDescriptor() const {
        if (!socket_) {
            return -1;
        } else {
            return static_cast<int>(socket_->native_handle());
        }
    }

    inline void setHeartTimeOut() {
            if (timer) {
                timer->cancel();
                delete timer;
                timer = 0;
            }
            if (socket_)
                timer = new boost::asio::deadline_timer(socket_->get_io_service(),boost::posix_time::seconds(q->timeOut_s));
        }

    boost::system::error_code erro_code;
protected:
    // 数据读取的回调函数
    void readHandler(const boost::system::error_code& error, std::size_t bytes_transferred);
    // 数据写入的回调函数
    void writeHandler(const boost::system::error_code& error,std::size_t bytes_transferred);
    // 链接之后的回调函数
    void connectedHandler(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator iter);
    // 解析主机后的回调函数
    void resolverHandle(const boost::system::error_code & error, boost::asio::ip::tcp::resolver::iterator iter);

    void heartTimeOutHandler(const boost::system::error_code& error);

private:
    QAsioTcpSocketParent * q;
    boost::asio::ip::tcp::socket * socket_;
    boost::asio::ip::tcp::resolver * resolver_;
    boost::asio::deadline_timer * timer;
    boost::asio::io_service::strand * stand_;
    int byteSize_;
    //接受数据的缓存
    char * data_;
};

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
            q->state_ = QAsioTcpSocketParent::UnconnectedState;
            q->erro_site = QAsioTcpSocketParent::NoError;
            if (socket_->is_open())
                socket_->close(erro_code);
            erro_code = error;
            if (timer)
                timer->cancel();
            q->haveErro();
            return;
        }
        q->readDataed(data_,bytes_transferred);
        if (timer) {
            timer->cancel();
            timer->async_wait(boost::bind(&QAsioTcpSocketParentPrivate::heartTimeOutHandler,boost::asio::placeholders::error));
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

void QAsioTcpSocketParentPrivate::connectToHost(const QString & hostName, quint16 port)
{
        if (!socket_){
            socket_ = new boost::asio::ip::tcp::socket(IOServerThread::getIOThread().getIoServer());
            if (stand_) {
                delete stand_;
            }
            stand_ = new boost::asio::io_service::strand(socket_->get_io_service());
            if (q->timeOut_s > 0) {
                setHeartTimeOut();
            }
        } else if (socket_->is_open()) {
            socket_->close(erro_code);
        }
        if (!resolver_)
            resolver_ = new boost::asio::ip::tcp::resolver(socket_->get_io_service());
        if (timer)
            timer->cancel();
//        resolver_->async_resolve(boost::asio::ip::tcp::resolver::query(hostName.toStdString(),QString::number(port).toStdString()),
//                                 boost::bind(&QAsioTcpSocketParentPrivate::resolverHandle,this, _1,_2));
        resolver_->async_resolve(boost::asio::ip::tcp::resolver::query(hostName.toStdString(),QString::number(port).toStdString()),
                                 std::bind(&QAsioTcpSocketParentPrivate::resolverHandle,this,std::placeholders::_1,std::placeholders::_2));
    }

void QAsioTcpSocketParentPrivate::writeHandler(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (!error && q->state_ == QAsioTcpSocketParent::ConnectedState){
        if (q->writeDataed(bytes_transferred))
            return;
    }
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

void QAsioTcpSocketParentPrivate::resolverHandle(const boost::system::error_code &error,boost::asio::ip::tcp::resolver::iterator iter)
{
    if (!error) {
        boost::asio::async_connect(
                    (*socket_),iter,
//                    /*stand_->wrap(*/boost::bind(&QAsioTcpSocketParentPrivate::connectedHandler, _1,_2)/*)*/);
                    std::bind(&QAsioTcpSocketParentPrivate::resolverHandle,this,std::placeholders::_1,std::placeholders::_2));
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
                timer->async_wait(boost::bind(&QAsioTcpSocketParentPrivate::heartTimeOutHandler,boost::asio::placeholders::error));
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
    QObject(parent),erro_site(NoError),timeOut_s(0),peerPort(0)
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
template<typename TcpSocket>
bool QAsioTcpSocketParent::setTcpSocket(TcpSocket *socket)
{
    return p->setTcpSocket(socket);
}

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
