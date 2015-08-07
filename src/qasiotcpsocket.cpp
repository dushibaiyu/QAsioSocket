#include "qasiotcpsocket.h"
#include "qsocketconnection.h"
#include <QMutex>

static QMutex mut;

IOServiceClass * QAsioTcpsocket::ioserver = nullptr;
volatile ulong QAsioTcpsocket::linkCout = 0;

QAsioTcpsocket::QAsioTcpsocket(size_t size,QObject *parent) :
    TcpAbstractSocket(parent),serverBuild(false)
{
    mut.lock();
    if (ioserver == nullptr) {
        ioserver = new IOServiceClass();
    }
    ++ linkCout;
    mut.unlock();
    con_ = new Session(ioserver->getService(),size);
    con_->connection->setQTcp(this);
}

QAsioTcpsocket::QAsioTcpsocket(Session * ses) :
    TcpAbstractSocket(0),serverBuild(true),con_(ses)
{
    int soc = con_->connection->socketDescriptor();
    if (soc != -1) {
        setState(ConnectedState);
        setSocketPtr(soc);
    }
    con_->connection->setQTcp(this);
}

QAsioTcpsocket::~QAsioTcpsocket()
{
    con_->connection->setQTcp(nullptr);
    delete con_;
    if (!serverBuild) {
        mut.lock();
        -- linkCout;
        if (linkCout == 0) {
            delete ioserver;
        }
        mut.unlock();
    }
}

void QAsioTcpsocket::do_start()
{
    if (serverBuild) {
        con_->connection->do_read();
    }
}

void QAsioTcpsocket::hostConnected()
{
    setState(ConnectedState);
    emit connected();
    emit stateChange(state());
    con_->connection->do_read();
}

void QAsioTcpsocket::connectToHost(const QString &hostName, quint16 port)
{
    if (state() != UnconnectedState)
        con_->connection->disconnectFromHost();
    setState(HostFinding);
    con_->connection->connectToHost(hostName,port);
}

void QAsioTcpsocket::disconnectFromHost()
{
    if (state() == UnconnectedState) return;
    con_->connection->disconnectFromHost();
}

void QAsioTcpsocket::sendData(const char * data, int size)
{
    if (state() == ConnectedState)
        con_->connection->wirteData(data,size);
}

void QAsioTcpsocket::setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble, uint value)
{
    con_->connection->setSocketOption(option,isEnble,value);
}

std::pair<bool,int> QAsioTcpsocket::getSocketOption(TcpAbstractSocket::SocketOption opention)
{
    return con_->connection->getSocketOption(opention);
}

int QAsioTcpsocket::resizeClientBackThreadSize(int size)
{
    if (serverBuild) return -1;
    return ioserver->setThreadSize(size);
}
