#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

//#include "qasioevent.h"
#include "qasiotcpsocket.h"
#include <QCoreApplication>
#include <QDebug>

const QEvent::Type QAsioEvent::QAsioSocketEventType = (QEvent::Type)QEvent::registerEventType();

QAsioTcpSocket::QAsioTcpSocket(int bytesize, QObject *parent) :
    QAsioTcpSocketParent(bytesize,parent),isDisconDelData(false)
{}

QAsioTcpSocket::~QAsioTcpSocket()
{
    willDelete();
}

bool QAsioTcpSocket::write(const QByteArray &data)
{
    if (state() != ConnectedState)
        return false;
    writeMutex.lock();
    if (!data.isEmpty())
        writeQueue.append(data);
    if (writeQueue.isEmpty()) {
        writeMutex.unlock();
        return false;
    }
    if (writeQueue.size() == 1) {
        wirteData(writeQueue.head().data(),static_cast<std::size_t>(writeQueue.head().size()));
    }
    writeMutex.unlock();
    return true;
}

void QAsioTcpSocket::customEvent(QEvent *event)
{
    if (event->type() == QAsioEvent::QAsioSocketEventType)
    {
        QAsioEvent * e = static_cast<QAsioEvent*>(event);
        switch (e->getConnectedType()) {
        case QAsioEvent::ReadReadly :
            if (isDisconDelData && state() != ConnectedState) break;
            emit readReadly();
            break;
        case QAsioEvent::Connected :
            emit stateChange(ConnectedState);
            emit connected();
            break;
        case QAsioEvent::HaveEorro :
            emit sentError(erroSite(),error());
            emit stateChange(UnconnectedState);
            emit disconnected();
            break;
        case QAsioEvent::DisConnect :
            emit stateChange(UnconnectedState);
            emit disconnected();
            break;
        case QAsioEvent::HostFined :
            emit hostFound();
            break;
        case QAsioEvent::HeartTimeOut:
            emit heartTimeOuted(e->getData().toInt());
            break;
        default:
            break;
        }
        e->accept();
    } else {
        QObject::customEvent(event);
    }
}

void QAsioTcpSocket::haveErro()
{
    switch (erroSite()) {
    case NoError:
        QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::DisConnect),Qt::HighEventPriority);
        break;
    case ReadError:
    case WriteEorro:
        if (error() == 2) {
            QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::DisConnect),Qt::HighEventPriority);
            break;
        }
    default:
        QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::HaveEorro),Qt::HighEventPriority);
    }
    if (isDisconDelData) {
        bufferMutex.lock();
        if (buffer.isOpen())
            buffer.close();
        bufferMutex.unlock();
    }
}

void QAsioTcpSocket::hostConnected()
{
    qDebug() << this << "  QAsioTcpSocket::hostConnected() peerIp:" << getPeerIp() << "  peerPort:" << getPeerPort();

    QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::Connected));
}

void QAsioTcpSocket::readDataed(const char * data,std::size_t bytes_transferred)
{
    bufferMutex.lock();
    if (buffer.isOpen() && buffer.atEnd())
        buffer.close();
    if (!buffer.isOpen())
        buffer.open(QBuffer::ReadWrite|QBuffer::Truncate);
    qint64 readPos = buffer.pos();
    buffer.seek(buffer.size());
    Q_ASSERT(buffer.atEnd());
    buffer.write(data, qint64(bytes_transferred));
    buffer.seek(readPos);
    bufferMutex.unlock();
    QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::ReadReadly));
}

bool QAsioTcpSocket::writeDataed(std::size_t bytes_transferred)
{
    writeMutex.lock();
    if (static_cast<std::size_t>(writeQueue.head().size()) == bytes_transferred){
        writeQueue.dequeue();
        if (!writeQueue.isEmpty()) {
            wirteData(writeQueue.head().data(),static_cast<std::size_t>(writeQueue.head().size()));
        }
        writeMutex.unlock();
        return true;
    } else {
        writeMutex.unlock();
        return false;
    }
}

void QAsioTcpSocket::finedHosted()
{
    QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::HostFined));
}

void QAsioTcpSocket::heartTimeOut(int timeout)
{
    QAsioEvent * e = new QAsioEvent(QAsioEvent::HeartTimeOut);
    e->setData(timeout);
    QCoreApplication::postEvent(this,e);
}

