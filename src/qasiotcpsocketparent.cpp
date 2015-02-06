#include "qasiotcpsocketparent.h"
#include <functional>

QAsioTcpSocketParent::QAsioTcpSocketParent(int byteSize, QObject *parent) :
    QObject(parent),byteSize_(byteSize)
{
    data_ = new char[byteSize];
    socket_ = new asio::ip::tcp::socket(IOServerThread::getIOThread()->getIOServer());
}

QAsioTcpSocketParent::QAsioTcpSocketParent(asio::ip::tcp::socket *socket,int byteSize, QObject *parent) :
    QObject(parent),socket_(socket),byteSize_(byteSize)
{
    //    asio::async_read(*socket_,asio::buffer(data_,byteSize_),
    //                     std::bind(&QAsioTcpSocket::readHandler,this,std::placeholders::_1,std::placeholders::_2));
    data_ = new char[byteSize];
    if (socket_->is_open()) {
        socket_->async_read_some(asio::buffer(data_,byteSize_),
                                 std::bind(&QAsioTcpSocketParent::readHandler,this,std::placeholders::_1,std::placeholders::_2));
        state_ = ConnectedState;
    }
}

QAsioTcpSocketParent::~QAsioTcpSocketParent()
{
    if (resolver_ != nullptr)
        delete resolver_;
    if (socket_ != nullptr) {
        if (socket_->is_open())
            socket_->close();
        delete socket_;
    }
    delete data_;
}

void QAsioTcpSocketParent::readHandler(const asio::error_code &error, std::size_t bytes_transferred)
{
    if(!error){
        if (bytes_transferred == 0){
            state_ = UnconnectedState;
            erro_site = NoError;
            socket_->close(erro_code);
            erro_code = error;
#ifdef ASIO_HAS_BOOST_DATE_TIME
            if (timer != nullptr)
                timer->cancel();
#endif
            haveErro();
            return;
        }
        readDataed(data_,bytes_transferred);
#ifdef ASIO_HAS_BOOST_DATE_TIME
        if (timer != nullptr) {
            timer->cancel();
            timer->async_wait(std::bind(&QAsioTcpSocketParent::heartTimeOutHandler,this,std::placeholders::_1));
        }
#endif
        socket_->async_read_some(asio::buffer(data_,byteSize_),
                                 std::bind(&QAsioTcpSocketParent::readHandler,this,std::placeholders::_1,std::placeholders::_2));

    } else {
        state_ = UnconnectedState;
        erro_site = ReadError;
        socket_->close(erro_code);
        erro_code = error;
#ifdef ASIO_HAS_BOOST_DATE_TIME
        if (timer != nullptr)
            timer->cancel();
#endif
        haveErro();
    }
}

void QAsioTcpSocketParent::writeHandler(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error){
        if (writeDataed(bytes_transferred)) return;
    }
    state_ = UnconnectedState;
    erro_site = WriteEorro;
    socket_->close(erro_code);
    erro_code = error;
#ifdef ASIO_HAS_BOOST_DATE_TIME
    if (timer != nullptr)
        timer->cancel();
#endif
    haveErro();
}

void QAsioTcpSocketParent::resolverHandle(const asio::error_code &error,asio::ip::tcp::resolver::iterator iterator)
{
    if (!error) {
        if (socket_ == nullptr) {
            socket_ = new asio::ip::tcp::socket(IOServerThread::getIOThread()->getIOServer());
        }
        asio::async_connect(*socket_,iterator,
                            std::bind(&QAsioTcpSocketParent::connectedHandler,this,std::placeholders::_1,std::placeholders::_2));
        finedHosted();
    } else {
        state_ = UnconnectedState;
        erro_site = FindHostError;
        erro_code = error;
#ifdef ASIO_HAS_BOOST_DATE_TIME
        if (timer != nullptr)
            timer->cancel();
#endif
        haveErro();
    }
}

void QAsioTcpSocketParent::connectToHost(const asio::ip::tcp::endpoint & peerPoint)
{
    if (resolver_ == nullptr)
        resolver_ = new asio::ip::tcp::resolver(socket_->get_io_service());
    resolver_->async_resolve(peerPoint,
                             std::bind(&QAsioTcpSocketParent::resolverHandle,this,std::placeholders::_1,std::placeholders::_2));
    state_ = ConnectingState;
    emit stateChange(ConnectingState);
}

void QAsioTcpSocketParent::connectToHost(const QString & hostName, quint16 port)
{
    if (resolver_ == nullptr)
        resolver_ = new asio::ip::tcp::resolver(socket_->get_io_service());
    resolver_->async_resolve(asio::ip::tcp::resolver::query(hostName.toStdString(),QString::number(port).toStdString()),
                             std::bind(&QAsioTcpSocketParent::resolverHandle,this,std::placeholders::_1,std::placeholders::_2));
    state_ = ConnectingState;
    emit stateChange(ConnectingState);
}

void QAsioTcpSocketParent::connectedHandler(const asio::error_code &error,asio::ip::tcp::resolver::iterator iterator)
{
    if (!error) {
        state_ = ConnectedState;
        erro_site = NoError;
        socket_->async_read_some(asio::buffer(data_,byteSize_),
                                 std::bind(&QAsioTcpSocketParent::readHandler,this,std::placeholders::_1,std::placeholders::_2));
        this->peerPoint = socket_->remote_endpoint(erro_code);
        hostConnected();
#ifdef ASIO_HAS_BOOST_DATE_TIME
        if (timer != nullptr)
            timer->async_wait(std::bind(&QAsioTcpSocketParent::heartTimeOutHandler,this,std::placeholders::_1));
#endif
    } else {
        asio::ip::tcp::resolver::iterator end;
        if(iterator == end)
        {
            state_ = UnconnectedState;
            erro_site = ConnectEorro;
            socket_->close(erro_code);
            erro_code = error;
#ifdef ASIO_HAS_BOOST_DATE_TIME
            if (timer != nullptr)
                timer->cancel();
#endif
            haveErro();
        }
    }
}

void QAsioTcpSocketParent::disconnectFromHost()
{
#ifdef ASIO_HAS_BOOST_DATE_TIME
    if (timer != nullptr)
        timer->cancel();
#endif
    if (state_ == UnconnectedState) return;
    socket_->shutdown(asio::ip::tcp::socket::shutdown_both);
}


