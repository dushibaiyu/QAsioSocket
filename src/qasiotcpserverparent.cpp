#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "qasiotcpserverparentprivate.h"

QAsioTcpServerParentPrivate::QAsioTcpServerParentPrivate(QAsioTcpServerParent * tq):
    q(tq),acceptor(0),socket_(0),stand_(0),iosService(IOServerThread::getIOThread())
{
    q->threadSize = iosService.setIoThreadSize(q->threadSize);
}

QAsioTcpServerParentPrivate::~QAsioTcpServerParentPrivate()
{
    close();
    if (acceptor) delete acceptor;
    if (stand_) delete stand_;
    if (socket_) delete socket_;
}

void QAsioTcpServerParentPrivate::appectHandle(const boost::system::error_code &code)
{
    if (!code) {
        q->incomingConnection();
    } else {
        error_ = code;
        if (q->haveErro()){
            q->type_ = QAsioTcpServerParent::None;
        }
    }
    if (q->type_ == QAsioTcpServerParent::None){
        close();
        return;
    }
    if (!socket_) {
        socket_ = new boost::asio::ip::tcp::socket(iosService.getIoServer());
    }
    acceptor->async_accept(*socket_,
                           stand_->wrap(boost::bind(&QAsioTcpServerParentPrivate::appectHandle,this,boost::asio::placeholders::error)));
}


QAsioTcpServerParent::QAsioTcpServerParent(int threadSize, QObject *parent)
    : QObject(parent),type_(None),threadSize(threadSize)
{
    if (this->threadSize <= 0) this->threadSize = 2;
    p = new QAsioTcpServerParentPrivate(this);
}

QAsioTcpServerParent::~QAsioTcpServerParent()
{
    delete p;
}

void QAsioTcpServerParent::close()
{
    p->close();
    type_ = None;
    ip_.clear();
}


bool QAsioTcpServerParent::listen(qint16 port, ListenType ltype)
{
    bool tmpbool = false;
    close();
#ifdef Q_OS_WIN
    switch (ltype) {  
    case IPV4 :
    {
        boost::asio::ip::tcp::endpoint endpot(boost::asio::ip::tcp::v4(),port);
        tmpbool = p->linstenAp(endpot);
    }
        break;
    case IPV6 :
    {
        boost::asio::ip::tcp::endpoint endpot(boost::asio::ip::tcp::v6(),port);
        tmpbool = p->linstenAp(endpot);
    }
        break;
    default:
        break;
    }
#else
    if (ltype == Both) {
        boost::asio::ip::tcp::endpoint endpot(boost::asio::ip::tcp::v4(),port);
        tmpbool = p->linstenAp(endpot);
    }
#endif
    type_ = ltype;
    ip_ = "0";
    if (!tmpbool)
        close();
    return tmpbool;
}

bool QAsioTcpServerParent::listen(const QString &ip, qint16 port)
{
    bool tmpbool = false;
    close();
    boost::system::error_code code;
    boost::asio::ip::address address = boost::asio::ip::address::from_string(ip.toStdString(),code);
    if (code) {
        p->error_ = code;
        tmpbool =  false;
    } else {
        ip_ = ip;
        boost::asio::ip::tcp::endpoint endpot(address,port);
        tmpbool = p->linstenAp(endpot);
    }
    if (!tmpbool) {
        close();
    } else {
#ifdef Q_OS_WIN
        if (address.is_v4()) {
            type_ = IPV4;
        } else {
            type_ = IPV6;
        }
#else
      type_ = Both;
#endif
    }
    return tmpbool;
}

bool QAsioTcpServerParent::setNewSocket(QAsioTcpSocketParent * socket)
{
    return p->setNewSocket(socket);
}

int QAsioTcpServerParent::getEorrorCode() const
{
    return p->error_.value();
}
