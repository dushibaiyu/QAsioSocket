#ifdef QASIO_SSL
#include "qasiosslserver.h"
#include "linstenclass.hpp"
#include "qsocketconnection.h"

class SslLinstenClass;

struct SSLCertificate
{
    QByteArray certificate;
    QByteArray privateKey;
    QByteArray dh512;
    QString password;
    asio::ssl::context ctx;
    SSLCertificate():ctx(asio::ssl::context::sslv23)
    {}
};

struct SSLSocketSession
{
    IOServiceClass io_;
    size_t size_;
    QAsioSslServer * server;
    SSLCertificate * sslCtx_;
    std::shared_ptr<SslLinstenClass> * listen_;
    SSLSocketSession(QAsioSslServer * ser,SSLCertificate * ssl,size_t size = 4096);
    ~SSLSocketSession();
    SSLSocketSession() = delete;
    SSLSession * haveNewAppent(SSLSession * son);
    void sharkHandle(SSLSession * ses) {
            server->incomingConnection(ses);
    }
    void refSocket();
    void doAppent();
};

class SslLinstenClass :
        public LinstenClass<SSLSession,SSLSocketSession>,
        public std::enable_shared_from_this<SslLinstenClass>,
        public asio::noncopyable
{
public :
    SslLinstenClass(SSLSession * connec, SSLSocketSession * tq,IO_Service &io_s) :
        LinstenClass<SSLSession,SSLSocketSession>(connec,tq,io_s)
    {}
    ~SslLinstenClass(){}

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
    inline void do_shanke(SSLSession * son){
        son->connection->sslSocket()
                .async_handshake(asio::ssl::stream_base::server,std::bind(&SslLinstenClass::sharkHandle,shared_from_this(),std::placeholders::_1,son));
    }
    void sharkHandle(const asio::error_code & code,SSLSession * ses) {
        if (!code && (q != nullptr)) {
            q->sharkHandle(ses);
        } else {
            delete ses;
        }
    }
    inline void setNewSockt(SSLSession * son) {
        SSLSession * tmp = this->connection;
        this->connection = son;
        if (tmp != nullptr) {
            delete tmp;
        }
    }

};

SSLSocketSession::SSLSocketSession(QAsioSslServer * ser, SSLCertificate *ssl, size_t size):size_(size),server(ser),sslCtx_(ssl),
        listen_(new std::shared_ptr<SslLinstenClass>(new SslLinstenClass(nullptr,this,io_.getService())))
{}

SSLSession * SSLSocketSession::haveNewAppent(SSLSession * son)
{
    SSLSession * newCon = new SSLSession(io_.getService(),sslCtx_->ctx,size_);
    (*listen_)->do_shanke(son);
    return newCon;
}

void SSLSocketSession::refSocket()
{
    SSLSession * newCon = new SSLSession(io_.getService(),sslCtx_->ctx,size_);
    (*listen_)->setNewSockt(newCon);
}

void SSLSocketSession::doAppent()
{
    (*listen_)->do_appent();
}

SSLSocketSession::~SSLSocketSession()
{
       (*listen_)->clear();
       delete listen_;
   }

QAsioSslServer::QAsioSslServer(size_t socket_buffer, QObject *parent) :
    QObject(parent),contx_(new SSLCertificate),
    listen_session(new SSLSocketSession(this,contx_,socket_buffer))
{
    contx_->ctx.set_password_callback(std::bind(&QAsioSslServer::sslGetPassword,this));
}

QAsioSslServer::~QAsioSslServer()
{
    close();
    delete contx_;
    delete listen_session;
}

void QAsioSslServer::incomingConnection(SSLSession * session)
{
    QAsioSslSocket * socket = new QAsioSslSocket(session);
    socket->moveToThread(this->thread());
    connect(this,&QAsioSslServer::deleteAllLink,socket,&QAsioSslSocket::deleteLater);
    emit newConnection(socket);
}

void QAsioSslServer::setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble, uint value)
{
    (*listen_session->listen_)->setSocketOption(option,isEnble,value);
}

bool QAsioSslServer::listen(const QString & ip,qint16 port)
{
    close();
    asio::error_code code;
    asio::ip::address address = asio::ip::address::from_string(ip.toStdString(),code);
    asio::ip::tcp::endpoint endpot(address,port);
    listen_session->refSocket();
    return (*listen_session->listen_)->linstenAp(endpot);
}

bool QAsioSslServer::listen(qint16 port,bool isV6)
{
    close();
    asio::ip::tcp::endpoint endpot;
    if (isV6)
        endpot = asio::ip::tcp::endpoint(asio::ip::tcp::v6(),port);
    else
        endpot = asio::ip::tcp::endpoint(asio::ip::tcp::v4(),port);
    listen_session->refSocket();
    return (*listen_session->listen_)->linstenAp(endpot);
}

int QAsioSslServer::setThreadSize(int  size)
{
    return listen_session->io_.setThreadSize(size);
}

void QAsioSslServer::close()
{
        (*listen_session->listen_)->close();
}

void QAsioSslServer::setCertificateChain(const QByteArray & data)
{
    contx_->certificate = data;
    contx_->ctx.use_certificate_chain(asio::buffer(contx_->certificate.data(),contx_->certificate.size()));
}

void QAsioSslServer::setPrivateKey(const QByteArray & data,KeyType type)
{
    contx_->privateKey = data;
    switch (type) {
    case ASN1:
        contx_->ctx.use_private_key(asio::buffer(contx_->privateKey.data(),contx_->privateKey.size()),asio::ssl::context::asn1);
        break;
    case PEM:
        contx_->ctx.use_private_key(asio::buffer(contx_->privateKey.data(),contx_->privateKey.size()),asio::ssl::context::pem);
        break;
    default:
        break;
    }
}

void QAsioSslServer::setDH(const QByteArray & data)
{
    contx_->dh512 = data;
    contx_->ctx.use_tmp_dh(asio::buffer(contx_->dh512.data(),contx_->dh512.size()));
}

void QAsioSslServer::setPassword(const QString & pass)
{
    contx_->password = pass;
}

void QAsioSslServer::setSslOption(SSLOption option)
{
    switch (option) {
    case Default_Workarounds:
        contx_->ctx.set_options(asio::ssl::context::default_workarounds);
        break;
    case No_Compression:
        contx_->ctx.set_options(asio::ssl::context::no_compression);
        break;
    case No_SSLv2:
        contx_->ctx.set_options(asio::ssl::context::no_sslv2);
        break;
    case No_SSLv3:
        contx_->ctx.set_options(asio::ssl::context::no_sslv3);
        break;
    case No_TLSv1:
        contx_->ctx.set_options(asio::ssl::context::no_tlsv1);
        break;
    case Single_DH_Use:
        contx_->ctx.set_options(asio::ssl::context::single_dh_use);
        break;
    default:
        break;
    }
}

std::string QAsioSslServer::sslGetPassword() const
{
    return contx_->password.toStdString();
}
#endif//asio_ssl
