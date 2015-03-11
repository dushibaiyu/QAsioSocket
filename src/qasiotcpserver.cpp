#ifdef Q_OS_WIN
#include<Mswsock.h>
#endif

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif


#include "../include/qasiotcpserver.h"
#include <QCoreApplication>

const QEvent::Type QAsioNewEvent::QAsioNewEventType = (QEvent::Type)QEvent::registerEventType();

QAsioTcpServer::QAsioTcpServer(int readSize,int ThreadSize, QObject *parent) :
    QAsioTcpServerParent(ThreadSize,parent),byteSize(readSize)
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
            socket = 0;
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
