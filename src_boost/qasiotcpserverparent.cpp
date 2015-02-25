#include "qasiotcpserverparent.h"
#include "ioserverthread.h"
#include "qasiotcpsocketparent.h"
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/system/error_code.hpp>
#include <vector>

class QAsioTcpServerParentPrivate
{
public:
    explicit QAsioTcpServerParentPrivate(QAsioTcpServerParent * tq);
    ~QAsioTcpServerParentPrivate();

    //监听acceptor
    inline bool linstenAp(const boost::asio::ip::tcp::endpoint & endpoint) {
        if (!acceptor) {
            acceptor = new boost::asio::ip::tcp::acceptor(iosList[lastState]->getIoServer());
            if (stand_) delete stand_;
            stand_ = new boost::asio::io_service::strand(acceptor->get_io_service());
        }
        boost::system::error_code code;
        acceptor->open(endpoint.protocol());
        acceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(false),code);
        acceptor->bind(endpoint,code);
        if (code)
        {
            this->error_ = code;
            return false;
        }
        acceptor->listen(boost::asio::socket_base::max_connections, code);
        if (code)
        {
            this->error_ = code;
            return false;
        }
        if (!socket_)
            socket_ = new boost::asio::ip::tcp::socket(iosList[lastState]->getIoServer());
        acceptor->async_accept(*socket_,
                               stand_->wrap(boost::bind(&QAsioTcpServerParentPrivate::appectHandle,this,boost::asio::placeholders::error)));
        return true;
    }

    inline bool setNewSocket(QAsioTcpSocketParent * socket)
    {
        bool istrue = socket->setTcpSocket(socket_);
        if (istrue) {
            socket_ = 0;
        }
        return istrue;
    }

    inline void close(){
        if (acceptor != nullptr) {
            if (acceptor->is_open()){
                acceptor->close();
            }
        }
    }

    boost::system::error_code error_;
protected:
    //切换新连接采用asio::io_service，采用公平队列，一次轮询
    inline void goForward(){
        if (q->IOSize == 0) {
            lastState = 0;
        } else {
            lastState ++;
            if (lastState == iosList.size())
                lastState = 0;
        } }
    //有新连接的回调
    void appectHandle(const boost::system::error_code & code);
private:
    QAsioTcpServerParent * q;
    boost::asio::ip::tcp::acceptor * acceptor;
    std::vector<IOServerThread *> iosList;
    boost::asio::ip::tcp::socket * socket_;
    boost::asio::io_service::strand * stand_;
    int lastState;
};

QAsioTcpServerParentPrivate::QAsioTcpServerParentPrivate(QAsioTcpServerParent * tq):
    q(tq),acceptor(0),socket_(0),lastState(0),stand_(0)
{
    for (int i = 0; i < q->IOSize; ++i)
    {
        IOServerThread * iost = new IOServerThread;
        iost->setIoThreadSize(q->OneIOThread);
        iosList.push_back(iost);
    }
}

QAsioTcpServerParentPrivate::~QAsioTcpServerParentPrivate()
{
    close();
    if (acceptor) delete acceptor;
    if (socket_) delete socket_;
    for (std::vector<IOServerThread *>::size_type i = 0; i < iosList.size(); ++i)
    {
        IOServerThread * thread = iosList[i];
        delete thread;
    }
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
        goForward();
        socket_ = new boost::asio::ip::tcp::socket(iosList[lastState]->getIoServer());
    }
    acceptor->async_accept(*socket_,
                           stand_->wrap(boost::bind(&QAsioTcpServerParentPrivate::appectHandle,this,boost::asio::placeholders::error)));
}


QAsioTcpServerParent::QAsioTcpServerParent( int OneIOThread,int IOSize, QObject *parent)
    : QObject(parent),type_(None),OneIOThread(OneIOThread),IOSize(IOSize)
{
    if (this->OneIOThread <= 0) this->OneIOThread = 2;
    if (this->IOSize <= 0) this->IOSize = 1;
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
      type_ = Botn;
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
