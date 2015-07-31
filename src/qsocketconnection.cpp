
#include "qsocketconnection.h"

void QSocketConnection::setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble, uint value)
{
    switch (option) {
    case TcpAbstractSocket::Multicast_Loopback :
    {
        asio::ip::multicast::enable_loopback option(isEnble);
        set_option(option);
    }
        break;
    case TcpAbstractSocket::Multicast_TTL :
    {
        asio::ip::multicast::hops option(value);
        set_option(option);
    }
        break;
    case TcpAbstractSocket::IP_TCP_NODelay :
    {
        asio::ip::tcp::no_delay option(isEnble);
        set_option(option);
    }
        break;
    case TcpAbstractSocket::Broadcast :
    {
        asio::socket_base::broadcast option(isEnble);
        set_option(option);
    }
        break;
    case TcpAbstractSocket::Linger :
    {
        asio::socket_base::linger option(isEnble, value);
        set_option(option);
    }
        break;
    case TcpAbstractSocket::Keep_Live :
    {
        asio::socket_base::keep_alive option(isEnble);
        set_option(option);
    }
        break;
    case TcpAbstractSocket::Receive_Buffer_Size :
    {
        asio::socket_base::receive_buffer_size option(value);
        set_option(option);
    }
        break;
    case TcpAbstractSocket::Receive_Low_Watermark :
    {
        asio::socket_base::receive_low_watermark option(value);
        set_option(option);
    }
        break;
    case TcpAbstractSocket::Reuse_Address :
    {
        asio::socket_base::reuse_address option(isEnble);
        set_option(option);
    }
        break;
    case TcpAbstractSocket::Send_Buffer_Size :
    {
        asio::socket_base::send_buffer_size option(value);
        set_option(option);
    }
        break;
    case TcpAbstractSocket::Send_Low_Watermark :
    {
        asio::socket_base::send_low_watermark option(value);
        set_option(option);
    }
        break;
    default:
        break;
    }
}


std::pair<bool,int> QSocketConnection::getSocketOption(TcpAbstractSocket::SocketOption option)
{
    switch (option) {
    case TcpAbstractSocket::Multicast_Loopback :
    {
        asio::ip::multicast::enable_loopback option;
        get_option(option);
        bool is_set = option.value();
        return std::make_pair(is_set,0);
    }
        break;
    case TcpAbstractSocket::Multicast_TTL :
    {
        asio::ip::multicast::hops option;
        get_option(option);
        int ttl = option.value();
        return std::make_pair(false,ttl);
    }
        break;
    case TcpAbstractSocket::IP_TCP_NODelay :
    {
        asio::ip::tcp::no_delay option;
        get_option(option);
        bool is_set = option.value();
        return std::make_pair(is_set,0);
    }
        break;
    case TcpAbstractSocket::Broadcast :
    {
        asio::socket_base::broadcast option;
        get_option(option);
        bool is_set = option.value();
        return std::make_pair(is_set,0);
    }
        break;
    case TcpAbstractSocket::Linger :
    {
        asio::socket_base::linger option;
        get_option(option);
        bool is_set = option.enabled();
        int timeout = option.timeout();
        return std::make_pair(is_set,timeout);
    }
        break;
    case TcpAbstractSocket::Keep_Live :
    {
        asio::socket_base::keep_alive option;
        get_option(option);
        bool is_set = option.value();
        return std::make_pair(is_set,0);
    }
        break;
    case TcpAbstractSocket::Receive_Buffer_Size :
    {
        asio::socket_base::receive_buffer_size option;
        get_option(option);
        int size = option.value();
        return std::make_pair(false,size);
    }
        break;
    case TcpAbstractSocket::Receive_Low_Watermark :
    {
        asio::socket_base::receive_low_watermark option;
        get_option(option);
        int size = option.value();
        return std::make_pair(false,size);
    }
        break;
    case TcpAbstractSocket::Reuse_Address :
    {
        asio::socket_base::reuse_address option;
        get_option(option);
        bool is_set = option.value();
        return std::make_pair(is_set,0);

    }
        break;
    case TcpAbstractSocket::Send_Buffer_Size :
    {
        asio::socket_base::send_buffer_size option;
        get_option(option);
        int size = option.value();
        return std::make_pair(false,size);
    }
        break;
    case TcpAbstractSocket::Send_Low_Watermark :
    {
        asio::socket_base::send_low_watermark option;
        get_option(option);
        int size = option.value();
        return std::make_pair(false,size);
    }
        break;
    default:
        return std::make_pair(false,0);
        break;
    }
}


void QSocketConnection::readHandler(const asio::error_code& error, std::size_t bytes_transferred)
{
    if(!error){
        if ((Qtcp != nullptr) && (Qtcp->readDataed(buffer_.data(),bytes_transferred))) {
            do_read();
        } else {
            disconnectFromHost();
        }
    } else {
        setError(error,QString("Read Data Erro : "));
    }
}


void QSocketConnection::writeHandler(const asio::error_code& error,std::size_t bytes_transferred,QByteArray * sdata)
{
    int size = sdata->size();
    delete sdata;
    if (!error && bytes_transferred > 0){
        if ((Qtcp != nullptr) && (!Qtcp->writeDataed(bytes_transferred,size)))
            disconnectFromHost();
    } else {
        setError(error,QString("Write Data Erro : "));
    }
}

void QSocketConnection::resolverHandle(const asio::error_code &error, asio::ip::tcp::resolver::iterator iter)
{
    if (!error && (Qtcp != nullptr)) {
        Qtcp->finedHosted();
        do_connect(iter);
    } else {
        setError(error,QString("Rresolver Domain erro :"));
    }
}


void QSocketConnection::connectedHandler(const asio::error_code& error, asio::ip::tcp::resolver::iterator iter)
{
    if (!error) {
        asio::ip::tcp::endpoint peerPoint;
#ifdef QASIO_SSL
        if (useSSL)
            peerPoint = sslSocketLty().remote_endpoint(erro_code);
        else
#endif
            peerPoint = socket().remote_endpoint(erro_code);

        if (Qtcp != nullptr) {
            Qtcp->hostConnected(socketDescriptor(),QString::fromStdString(peerPoint.address().to_string()),peerPoint.port());
        }
    } else {
        asio::ip::tcp::resolver::iterator end;
        if(iter == end) {
            setError(error,QString("connect to host erro :"));
        }
    }
}



void QSocketConnection::handshakeHandle(const asio::error_code & error)
{
    if (!error) {
        if (Qtcp != nullptr) Qtcp->handshaked();
    } else {
        setError(error,QString("handshake erro : "));
    }
}



