#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "qasiotcpserverparentprivate.h"

QAsioTcpServerParentPrivate::QAsioTcpServerParentPrivate(QAsioTcpServerParent * tq):
    q(tq),acceptor(0),socket_(0)
{
    for (int i = 0; i < q->threadSize; ++i)
    {
        IOServerThread * thread = new IOServerThread();
        thread->start();
        iosserverList.push_back(thread);
    }
}

QAsioTcpServerParentPrivate::~QAsioTcpServerParentPrivate()
{
    close();
    if (acceptor) delete acceptor;
    if (socket_) delete socket_;
    for (std::size_t i = 0; i < iosserverList.size(); ++i)
    {
        auto thread = iosserverList[i];
        delete thread;
    }
}

void QAsioTcpServerParentPrivate::appectHandle(const asio::error_code &code)
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
        goForward();
        socket_ = new asio::ip::tcp::socket(iosserverList[lastState]->getIOServer());
    } else {
        asio::error_code tcode_;
        socket_->close(tcode_);
    }
    acceptor->async_accept(*socket_,std::bind(&QAsioTcpServerParentPrivate::appectHandle,this,std::placeholders::_1));
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
#ifdef Q_OS_LINUX
    ltype = Both;
#endif
    switch (ltype) {
    case IPV4 :
    {
        asio::ip::tcp::endpoint endpot(asio::ip::tcp::v4(),port);
        tmpbool = p->linstenAp(endpot);
    }
        break;
    case IPV6 :
    {
        asio::ip::tcp::endpoint endpot(asio::ip::tcp::v6(),port);
        tmpbool = p->linstenAp(endpot);
    }
        break;
    case Both :
        {
            asio::ip::tcp::endpoint endpot(asio::ip::tcp::all(),port);
            tmpbool = p->linstenAp(endpot);
        }
    default:
        break;
    }
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
    asio::error_code code;
    asio::ip::address address = asio::ip::address::from_string(ip.toStdString(),code);
    if (code) {
        p->error_ = code;
        tmpbool =  false;
    } else {
        ip_ = ip;
        asio::ip::tcp::endpoint endpot(address,port);
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
