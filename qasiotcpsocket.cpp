#include "qasioevent.h"
#include <functional>
#include <QCoreApplication>

QAsioTcpSocket::QAsioTcpSocket(QObject *parent) :
    QObject(parent),state_(UnconnectedState)
{
    if(ioserver.isNull())
    {
        ioserver = QSharedPointer<IOServerThread>(new IOServerThread);
        ioserver->start();
    }
    socket_ = new asio::ip::tcp::socket(*ioserver);
}

QAsioTcpSocket::QAsioTcpSocket(asio::ip::tcp::socket * socket, QObject *parent) :
    QObject(parent),socket_(socket)
{
    socket_->async_read_some(asio::buffer(data_),
                             std::bind(&QAsioTcpSocket::readHandler,this,std::placeholders::_1,std::placeholders::_2));
    state_ = ConnectedState;
}

void QAsioTcpSocket::readHandler(const asio::error_code &error, std::size_t bytes_transferred)
{
    if(!error){
        if (bytes_transferred == 0){
            state_ = UnconnectedState;
            QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::DisConnect,error));
        }
        bufferMutex.lock();
        if (buffer->atEnd())
            buffer.close();
        if (!buffer.isOpen())
            buffer.open(QBuffer::ReadWrite|QBuffer::Truncate);
        qint64 readPos = buffer.pos();
        buffer.seek(buffer.size());
        Q_ASSERT(buffer.atEnd());
        buffer.write(data_.c_str(), qint64(bytes_transferred));
        data_.clear();
        buffer.seek(readPos);
        bufferMutex.unlock();
        socket_->async_read_some(asio::buffer(data_),
                                 std::bind(&QAsioTcpSocket::readHandler,this,std::placeholders::_1,std::placeholders::_2));
        QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::ReadReadly,error));
    } else {
        state_ = UnconnectedState;
        socket_->close();
        QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::ReadError,error));
    }
}

void QAsioTcpSocket::writeHandler(const asio::error_code &error, std::size_t bytes_transferred)
{
    if (!error){
        writeMutex.lock();
        if (writeQueue.head().size() == bytes_transferred){
            writeQueue.dequeue();
            writeMutex.unlock();
            if (!writeQueue.isEmpty())
                write();
            return;
        } else {
            writeMutex.unlock();
        }
    }
    state_ = UnconnectedState;
    socket_->close();
    QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::WriteEorro,error));
}

void QAsioTcpSocket::connectedHandler(const asio::error_code &error)
{
    if (!error) {
        state_ = ConnectedState;
        QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::Connected,error));
    } else {
        state_ = UnconnectedState;
        QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::ConnectEorro,error));
    }
}

bool QAsioTcpSocket::write(const QByteArray &data)
{
    if (state_ != ConnectedState)
        return false;
    writeMutex.lock();
    if (!data.isEmpty())
        writeQueue.append(data);
//    socket_->async_write_some(asio::buffer(writeQueue.head().data(),writeQueue.head().size()),
//                              std::bind(&QAsioTcpSocket::writeHandler,this,std::placeholders::_1,std::placeholders::_2));
    socket_->async_send(asio::buffer(writeQueue.head().data(),writeQueue.head().size()),
                        std::bind(&QAsioTcpSocket::writeHandler,this,std::placeholders::_1,std::placeholders::_2));
//    asio::async_write(*socket_,asio::buffer(writeQueue.head().data(),writeQueue.head().size()),
//                      std::bind(&QAsioTcpSocket::writeHandler,this,std::placeholders::_1,std::placeholders::_2));
    writeMutex.unlock();
    return true;
}

void QAsioTcpSocket::customEvent(QEvent *event)
{
    if (event->type() == QAsioEvent::QAsioSocketEventType)
    {
        auto e = static_cast<QAsioEvent*>(event);
        switch (e->getConnectedType()) {
        case QAsioEvent::ReadReadly :
            emit readReadly();
            break;
        case QAsioEvent::Connected :
            emit stateChange(ConnectedState);
            emit connected();
            break;
        case QAsioEvent::ConnectEorro :
            emit error(ConnectEorro,e->getErrorCode());
            emit stateChange(UnconnectedState);
            break;
        case QAsioEvent::DisConnect :
            emit stateChange(UnconnectedState);
            emit disconnected();
            break;
        case QAsioEvent::WriteEorro :
            emit error(WriteEorro,e->getErrorCode());
            emit stateChange(UnconnectedState);
            emit disconnected();
            break;
        case QAsioEvent::ReadError :
            emit error(ReadError,e->getErrorCode());
            emit stateChange(UnconnectedState);
            emit disconnected();
            break;
        default:
            break;
        }
        e->accept();
    }
}

void QAsioTcpSocket::connectToHost(const asio::ip::tcp::endpoint & peerPoint)
{
    socket_->async_connect(peerPoint,std::bind(&QAsioTcpSocket::connectedHandler,this,std::placeholders::_1));
}

void QAsioTcpSocket::connectToHost(const QString & hostName, quint16 port,, QAsioTcpSocket::AdressType type)
{
    switch (type) {
    case IPV4:

        break;
    case IPV6:

        break;
    default:
        break;
    }
}
