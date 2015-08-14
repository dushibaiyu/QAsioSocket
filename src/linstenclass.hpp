#ifndef LINSTENCLASS_H
#define LINSTENCLASS_H

#include "ioserviceclass.h"
#include "qsocketconnection.h"

template <typename SESSION,typename T>
class LinstenClass
{
public:
    LinstenClass(SESSION * connec, T * tq,IO_Service &io_s);
    virtual ~LinstenClass();

    void close(){
        if (acceptor->is_open()){
            acceptor->close();
        }
    }

    bool linsten(const asio::ip::tcp::endpoint & endpoint) {
        acceptor->open(endpoint.protocol());
        acceptor->bind(endpoint,erro_code);
        if (erro_code)
            return false;
        acceptor->listen(asio::socket_base::max_connections, erro_code);
        if (erro_code){
            close();
            return false;
        }
//        do_appent();
        return true;
    }

    void setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble, uint value);
    std::pair<bool,int> getSocketOption(TcpAbstractSocket::SocketOption);

    inline void clear() {q = nullptr;}

    asio::error_code erro_code;

    void appectHandle(const asio::error_code & code);

//    inline void do_appent(){
//        acceptor->async_accept(connection->socket(),
//                               stand_->wrap(std::bind(&LinstenClass::appectHandle,this,std::placeholders::_1)));
//    }
protected:
    asio::ip::tcp::acceptor * acceptor;
    asio::io_service::strand * stand_;
    SESSION * connection = nullptr;
    /*volatile*/ T * q = nullptr;
};

template <typename SESSION,typename T>
LinstenClass<SESSION,T>::LinstenClass(SESSION * connec, T * tq, IO_Service &io_s) :
    acceptor(new asio::ip::tcp::acceptor(io_s)),
    stand_(new asio::io_service::strand(io_s)),connection(connec),q(tq)
{
}

template <typename SESSION,typename T>
LinstenClass<SESSION,T>::~LinstenClass()
{
    if (connection != nullptr) delete connection;
}

template <typename SESSION,typename T>
void LinstenClass<SESSION,T>::appectHandle(const asio::error_code & code)
{
//    qDebug() << " ?? appectHandle : " << q;
    if (!code) {
        if (q != nullptr) {
            connection = q->haveNewAppent(connection);
            q->doAppent();
        } else {
            close();
        }
    } else {
        connection->connection->disconnectFromHost();
    }

}

template <typename SESSION,typename T>
void LinstenClass<SESSION,T>::setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble, uint value)
{
    switch (option) {
    case TcpAbstractSocket::Multicast_Loopback :
    {
        asio::ip::multicast::enable_loopback option(isEnble);
        acceptor->set_option(option);
    }
        break;
    case TcpAbstractSocket::Multicast_TTL :
    {
        asio::ip::multicast::hops option(value);
        acceptor->set_option(option);
    }
        break;
    case TcpAbstractSocket::IP_TCP_NODelay :
    {
        asio::ip::tcp::no_delay option(isEnble);
        acceptor->set_option(option);
    }
        break;
    case TcpAbstractSocket::Broadcast :
    {
        asio::socket_base::broadcast option(isEnble);
        acceptor->set_option(option);
    }
        break;
    case TcpAbstractSocket::Linger :
    {
        asio::socket_base::linger option(isEnble, value);
        acceptor->set_option(option);
    }
        break;
    case TcpAbstractSocket::Keep_Live :
    {
        asio::socket_base::keep_alive option(isEnble);
        acceptor->set_option(option);
    }
        break;
    case TcpAbstractSocket::Receive_Buffer_Size :
    {
        asio::socket_base::receive_buffer_size option(value);
        acceptor->set_option(option);
    }
        break;
    case TcpAbstractSocket::Receive_Low_Watermark :
    {
        asio::socket_base::receive_low_watermark option(value);
        acceptor->set_option(option);
    }
        break;
    case TcpAbstractSocket::Reuse_Address :
    {
        asio::socket_base::reuse_address option(isEnble);
        acceptor->set_option(option);
    }
        break;
    case TcpAbstractSocket::Send_Buffer_Size :
    {
        asio::socket_base::send_buffer_size option(value);
        acceptor->set_option(option);
    }
        break;
    case TcpAbstractSocket::Send_Low_Watermark :
    {
        asio::socket_base::send_low_watermark option(value);
        acceptor->set_option(option);
    }
        break;
    default:
        break;
    }
}
template <typename SESSION,typename T>
std::pair<bool,int> LinstenClass<SESSION,T>::getSocketOption(TcpAbstractSocket::SocketOption option)
{
    switch (option) {
    case TcpAbstractSocket::Multicast_Loopback :
    {
        asio::ip::multicast::enable_loopback option;
        acceptor->get_option(option);
        bool is_set = option.value();
        return std::make_pair(is_set,0);
    }
        break;
    case TcpAbstractSocket::Multicast_TTL :
    {
        asio::ip::multicast::hops option;
        acceptor->get_option(option);
        int ttl = option.value();
        return std::make_pair(false,ttl);
    }
        break;
    case TcpAbstractSocket::IP_TCP_NODelay :
    {
        asio::ip::tcp::no_delay option;
        acceptor->get_option(option);
        bool is_set = option.value();
        return std::make_pair(is_set,0);
    }
        break;
    case TcpAbstractSocket::Broadcast :
    {
        asio::socket_base::broadcast option;
        acceptor->get_option(option);
        bool is_set = option.value();
        return std::make_pair(is_set,0);
    }
        break;
    case TcpAbstractSocket::Linger :
    {
        asio::socket_base::linger option;
        acceptor->get_option(option);
        bool is_set = option.enabled();
        int timeout = option.timeout();
        return std::make_pair(is_set,timeout);
    }
        break;
    case TcpAbstractSocket::Keep_Live :
    {
        asio::socket_base::keep_alive option;
        acceptor->get_option(option);
        bool is_set = option.value();
        return std::make_pair(is_set,0);
    }
        break;
    case TcpAbstractSocket::Receive_Buffer_Size :
    {
        asio::socket_base::receive_buffer_size option;
        acceptor->get_option(option);
        int size = option.value();
        return std::make_pair(false,size);
    }
        break;
    case TcpAbstractSocket::Receive_Low_Watermark :
    {
        asio::socket_base::receive_low_watermark option;
        acceptor->get_option(option);
        int size = option.value();
        return std::make_pair(false,size);
    }
        break;
    case TcpAbstractSocket::Reuse_Address :
    {
        asio::socket_base::reuse_address option;
        acceptor->get_option(option);
        bool is_set = option.value();
        return std::make_pair(is_set,0);

    }
        break;
    case TcpAbstractSocket::Send_Buffer_Size :
    {
        asio::socket_base::send_buffer_size option;
        acceptor->get_option(option);
        int size = option.value();
        return std::make_pair(false,size);
    }
        break;
    case TcpAbstractSocket::Send_Low_Watermark :
    {
        asio::socket_base::send_low_watermark option;
        acceptor->get_option(option);
        int size = option.value();
        return std::make_pair(false,size);
    }
        break;
    default:
        return std::make_pair(false,0);
        break;
    }
}


#endif // LINSTENCLASS_H
