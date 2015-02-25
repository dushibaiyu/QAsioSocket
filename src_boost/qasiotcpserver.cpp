#ifdef Q_OS_WIN
#include<Mswsock.h>
#endif

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif


#include "qasiotcpserver.h"
#include "qasiotcpsocket.h"
#include <QCoreApplication>

//asio事件循环线程与server主线程有新连接交互的事件
class QAsioNewEvent : public QEvent
{
public:
    explicit QAsioNewEvent(QAsioTcpSocket * socket)
        :QEvent(QAsioNewEventType),socket_(socket){}
    static const QEvent::Type QAsioNewEventType;// = (QEvent::Type)QEvent::registerEventType();

    QAsioTcpSocket * getNewSocket() const {return socket_;}
private:
    QAsioTcpSocket * socket_;
};

const QEvent::Type QAsioNewEvent::QAsioNewEventType = (QEvent::Type)QEvent::registerEventType();

QAsioTcpServer::QAsioTcpServer(int readSize,int OneIOThread,int IOSize, QObject *parent) :
    QAsioTcpServerParent(OneIOThread,IOSize,parent),byteSize(readSize)
{}

QAsioTcpServer::~QAsioTcpServer()
{}

void QAsioTcpServer::customEvent(QEvent *e)
{
    if (e->type() == QAsioNewEvent::QAsioNewEventType)
    {
        QAsioNewEvent * newcon = static_cast<QAsioNewEvent *>(e);
        QAsioTcpSocket * socket = newcon->getNewSocket();
        if (socket->state() == QAsioTcpSocket::ConnectedState) {
            socket->setDisconnecdDeleteBuffer(true);
            emit newConnection(socket);;
        } else {
            delete socket;
        }
        e->accept();
    } else {
        QObject::customEvent(e);
    }
}

void QAsioTcpServer::incomingConnection()
{
    QAsioTcpSocket * asiosocket = new QAsioTcpSocket(byteSize);
    if (setNewSocket(asiosocket)) {
        asiosocket->moveToThread(this->thread());
        QCoreApplication::postEvent(this,new QAsioNewEvent(asiosocket),Qt::HighEventPriority);
    } else {
        delete asiosocket;
    }
}

bool QAsioTcpServer::haveErro()
{
    return true;
}
