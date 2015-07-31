#ifndef QSOCKRTCONNECTION_H
#define QSOCKRTCONNECTION_H

#include <atomic>
#include <functional>
#include "tcpabstractsocket.h"
#include "ioserviceclass.h"

#ifdef QASIO_SSL
#include <openssl/evp.h>
#include <openssl/ssl.h>
#include "asio/ssl.hpp"
typedef  asio::ssl::stream <asio::ip::tcp::socket> SSLSocket;
#endif

class QSocketConnection :
        public std::enable_shared_from_this<QSocketConnection> ,
        public asio::noncopyable
{
public :
    union Sockets {
        asio::ip::tcp::socket * nossl;
#ifdef QASIO_SSL
        SSLSocket * ssl;
#endif
    };
public:
    QSocketConnection(asio::io_service & service,size_t size) :
        stand_(new asio::io_service::strand(service)) ,useSSL(false)
    {
        socket_.nossl = new asio::ip::tcp::socket(service);
        buffer_.resize(size);
        qDebug() << buffer_.size();
    }
#ifdef QASIO_SSL
    QSocketConnection(asio::io_service & service,asio::ssl::context & ctx,size_t size) :
        stand_(new asio::io_service::strand(service)) ,useSSL(true)
    {
        socket_.ssl = new SSLSocket(service,ctx);
        buffer_.resize(size);
    }
#endif
    virtual ~QSocketConnection() {
        disconnectFromHost();
        delete resolver_;
//        qDebug() << "~QSocketConnection() {";
    }

    inline void connectToHost(const QString & hostName, quint16 port) {
        if (resolver_ == nullptr)
            resolver_ = new asio::ip::tcp::resolver(stand_->get_io_service());
        resolver_->async_resolve(asio::ip::tcp::resolver::query(hostName.toStdString(),QString::number(port).toStdString()),
                                 stand_->wrap(std::bind(&QSocketConnection::resolverHandle,
                                                        shared_from_this(), std::placeholders::_1,std::placeholders::_2)));
    }

    inline  void wirteData(QByteArray && data) {
        if (is_open()) {
            QByteArray * tdata = new QByteArray(data);
#ifdef QASIO_SSL
            if (useSSL)
                asio::async_write(sslSocket(),asio::buffer(tdata->data(),tdata->size()),
                                  stand_->wrap(std::bind(&QSocketConnection::writeHandler,shared_from_this(),
                                                         std::placeholders::_1,std::placeholders::_2,tdata)));
            else
#endif
                asio::async_write(socket(),asio::buffer(tdata->data(),tdata->size()),
                                  stand_->wrap(std::bind(&QSocketConnection::writeHandler,shared_from_this(),
                                                         std::placeholders::_1,std::placeholders::_2,tdata)));

        }
    }

    inline void disconnectFromHost() {
        if (resolver_ != nullptr) resolver_->cancel();
#ifdef QASIO_SSL
        if (useSSL) {
            if (sslSocketLty().is_open()) {
                sslSocket().shutdown(erro_code);
                sslSocketLty().close();
            }
        } else {
#endif
            if (socket().is_open()) {
                socket().shutdown(asio::ip::tcp::socket::shutdown_both,erro_code);
                socket().close();
            }
#ifdef QASIO_SSL
        }
#endif
    }


    inline void do_read() {
#ifdef QASIO_SSL
        if (useSSL)
            sslSocket().async_read_some(asio::buffer(buffer_,buffer_.size()),
                                        stand_->wrap(std::bind(&QSocketConnection::readHandler,shared_from_this(),
                                                               std::placeholders::_1,std::placeholders::_2)));
        else
#endif
            socket().async_read_some(asio::buffer(buffer_,buffer_.size()),
                                     stand_->wrap(std::bind(&QSocketConnection::readHandler,shared_from_this(),
                                                            std::placeholders::_1,std::placeholders::_2)));


    }
#ifdef QASIO_SSL
    inline void do_handshake(bool isServe) {
        socket_.ssl->async_handshake((isServe ? asio::ssl::stream_base::server :asio::ssl::stream_base::client )
                                     ,std::bind(&QSocketConnection::handshakeHandle,this,std::placeholders::_1));
    }
#endif

    void do_connect(asio::ip::tcp::resolver::iterator & iter) {
#ifdef QASIO_SSL
        if (useSSL)
            asio::async_connect(sslSocketLty(),iter,
                                stand_->wrap(std::bind(&QSocketConnection::connectedHandler,
                                                       shared_from_this(),std::placeholders::_1,std::placeholders::_2)));
        else
#endif
            asio::async_connect(socket(),iter,
                                stand_->wrap(std::bind(&QSocketConnection::connectedHandler,
                                                       shared_from_this(),std::placeholders::_1,std::placeholders::_2)));

    }

    inline bool is_open() {
#ifdef QASIO_SSL
        if (useSSL)
            return sslSocketLty().is_open();
        else
#endif
            return socket().is_open();
    }

    inline  int socketDescriptor() {
#ifdef QASIO_SSL
        if (useSSL)
            return static_cast<int>(sslSocketLty().native_handle());
        else
#endif
            return static_cast<int>(socket().native_handle());
    }

    void setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble, uint value);

    std::pair<bool,int> getSocketOption(TcpAbstractSocket::SocketOption option);

    inline void setQTcp(TcpAbstractSocket * qp) { Qtcp = qp; }
#ifdef QASIO_SSL
    inline SSLSocket::lowest_layer_type & sslSocketLty(){
        return socket_.ssl->lowest_layer();
    }

    inline  SSLSocket & sslSocket(){
        return *(socket_.ssl);
    }
#endif
    inline asio::ip::tcp::socket & socket(){
        return *(socket_.nossl);
    }
protected:
    // 数据读取的回调函数
    void readHandler(const asio::error_code& error, std::size_t bytes_transferred);
    // 数据写入的回调函数
    void writeHandler(const asio::error_code& error, std::size_t bytes_transferred, QByteArray * sdata);
    // 链接之后的回调函数
    void connectedHandler(const asio::error_code& error, asio::ip::tcp::resolver::iterator iter);
    // 解析主机后的回调函数
    void resolverHandle(const asio::error_code & error, asio::ip::tcp::resolver::iterator iter);

    void handshakeHandle(const asio::error_code & error);
protected:
    inline void setError(const asio::error_code & erro, QString && erro_string){
        erro_code = erro;
        erro_string.append(erro_code.message().c_str());
        disconnectFromHost();
        if (Qtcp != nullptr) {
            Qtcp->haveErro(erro.value(),erro_string);
        }
    }

    template <typename SettableSocketOption>
    void  set_option(const SettableSocketOption& option){
#ifdef QASIO_SSL
        if (useSSL)
            sslSocketLty().set_option(option,erro_code);
        else
#endif
            socket().set_option(option,erro_code);
    }

    template <typename SettableSocketOption>
    void get_option(SettableSocketOption & option) {
#ifdef QASIO_SSL
        if (useSSL)
            sslSocketLty().get_option(option,erro_code);
        else
#endif
            socket().get_option(option,erro_code);
    }


protected :
    asio::error_code erro_code;
    asio::ip::tcp::resolver * resolver_ = nullptr;
    std::vector<char> buffer_;
    asio::io_service::strand * stand_;
    /*std::atomic<*/ TcpAbstractSocket * /*>*/ Qtcp = nullptr;
    bool useSSL;
    Sockets socket_;
private:
    QSocketConnection(const QSocketConnection &) = delete;
};

#ifdef QASIO_SSL
struct SSLSession
{
    std::shared_ptr<QSocketConnection> connection;
    SSLSocket::lowest_layer_type & socket(){return connection->sslSocketLty();}
    SSLSession(asio::io_service & service,asio::ssl::context & ctx,size_t size = 4096) :
        connection(new QSocketConnection(service,ctx,size))
    {}
    ~SSLSession(){
        connection->setQTcp(nullptr);
    }
};
#endif

struct Session
{
    std::shared_ptr<QSocketConnection> connection;
    asio::ip::tcp::socket & socket() {return connection->socket();}
    Session(asio::io_service & server,size_t size = 4096) :
        connection(new QSocketConnection(server,size))
    {}
    ~Session() {
        connection->setQTcp(nullptr);
    }
};

#endif // QSOCKRTCONNECTION_H
