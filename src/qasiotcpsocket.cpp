#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "qasioevent.h"
#include <functional>
#include <QCoreApplication>
#include <QDebug>

QAsioTcpSocket::QAsioTcpSocket(QObject *parent) :
    QObject(parent),state_(UnconnectedState)
{
    socket_ = new asio::ip::tcp::socket(IOServerThread::getIOThread()->getIOServer());
}

QAsioTcpSocket::QAsioTcpSocket(asio::ip::tcp::socket * socket, QObject *parent) :
    QObject(parent),socket_(socket)
{
//    asio::async_read(*socket_,asio::buffer(data_,data_.max_size()),
//                     std::bind(&QAsioTcpSocket::readHandler,this,std::placeholders::_1,std::placeholders::_2));
    socket_->async_read_some(asio::buffer(data_,data_.max_size()),
                             std::bind(&QAsioTcpSocket::readHandler,this,std::placeholders::_1,std::placeholders::_2));
    state_ = ConnectedState;
}

QAsioTcpSocket::~QAsioTcpSocket()
{
    if (resolver_ != nullptr)
        delete resolver_;
    if (socket_ != nullptr) {
        if (socket_->is_open())
            socket_->close();
        delete socket_;
    }
}

void QAsioTcpSocket::readHandler(const asio::error_code &error, std::size_t bytes_transferred)
{
    if(!error){
//        qDebug() << "readHandler" << bytes_transferred;
        if (bytes_transferred == 0){
            state_ = UnconnectedState;
            socket_->close();
            QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::DisConnect,error));
            return;
        }
        bufferMutex.lock();
        if (buffer.atEnd())
            buffer.close();
        if (!buffer.isOpen())
            buffer.open(QBuffer::ReadWrite|QBuffer::Truncate);
        qint64 readPos = buffer.pos();
        buffer.seek(buffer.size());
        Q_ASSERT(buffer.atEnd());
        buffer.write(data_.data(), qint64(bytes_transferred));
        buffer.seek(readPos);
        bufferMutex.unlock();
        socket_->async_read_some(asio::buffer(data_,data_.max_size()),
                                 std::bind(&QAsioTcpSocket::readHandler,this,std::placeholders::_1,std::placeholders::_2));
        QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::ReadReadly,error));
    } else {
        state_ = UnconnectedState;
        socket_->close();
        if (error.value() == 2) {
            QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::DisConnect,error));
        } else {
            QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::ReadError,error));
        }
    }
}

void QAsioTcpSocket::writeHandler(const asio::error_code &error, std::size_t bytes_transferred)
{
//    qDebug() << "writeHandler" << bytes_transferred;
    if (!error){
        writeMutex.lock();
        if (static_cast<std::size_t>(writeQueue.head().size()) == bytes_transferred){
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
    if (error.value() == 2) {
        QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::DisConnect,error));
    } else {
        QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::WriteEorro,error));
    }
}

bool QAsioTcpSocket::write(const QByteArray &data)
{
    if (state_ != ConnectedState)
        return false;
    writeMutex.lock();
    if (!data.isEmpty())
        writeQueue.append(data);
    if (writeQueue.isEmpty()) {
        writeMutex.unlock();
        return false;
    }
    asio::async_write(*socket_,asio::buffer(writeQueue.head().data(),writeQueue.head().size()),
                      std::bind(&QAsioTcpSocket::writeHandler,this,std::placeholders::_1,std::placeholders::_2));
    //    socket_->async_send(asio::buffer(writeQueue.head().data(),writeQueue.head().size()),
    //                        std::bind(&QAsioTcpSocket::writeHandler,this,std::placeholders::_1,std::placeholders::_2));
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
            emit sentError(tr("连接到错误"),e->getErrorCode());
            emit stateChange(UnconnectedState);
            break;
        case QAsioEvent::DisConnect :
            emit stateChange(UnconnectedState);
            emit disconnected();
            break;
        case QAsioEvent::WriteEorro :
            emit sentError(tr("写入Socket错误"),e->getErrorCode());
            emit stateChange(UnconnectedState);
            emit disconnected();
            break;
        case QAsioEvent::ReadError :
            emit sentError(tr("读取数据错误"),e->getErrorCode());
            emit stateChange(UnconnectedState);
            emit disconnected();
            break;
        case QAsioEvent::FindHosted :
            emit hostFound();
            break;
        case QAsioEvent::FindHostEorro :
            emit sentError(tr("查找服务器错误"),e->getErrorCode());
            emit stateChange(UnconnectedState);
            break;
        default:
            break;
        }
        e->accept();
    } else {
        QObject::customEvent(event);
    }
}

void QAsioTcpSocket::resolverHandle(const asio::error_code &error,asio::ip::tcp::resolver::iterator iterator)
{
    if (!error) {
        if (socket_ == nullptr) {
            socket_ = new asio::ip::tcp::socket(IOServerThread::getIOThread()->getIOServer());
        }
        asio::async_connect(*socket_,iterator,
                      std::bind(&QAsioTcpSocket::connectedHandler,this,std::placeholders::_1,std::placeholders::_2));
        QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::FindHosted,error));
    } else {
        state_ = UnconnectedState;
        QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::FindHostEorro,error));
    }
}

void QAsioTcpSocket::connectToHost(const asio::ip::tcp::endpoint & peerPoint)
{
    if (resolver_ == nullptr)
        resolver_ = new asio::ip::tcp::resolver(socket_->get_io_service());
    resolver_->async_resolve(peerPoint,
                           std::bind(&QAsioTcpSocket::resolverHandle,this,std::placeholders::_1,std::placeholders::_2));
    state_ = ConnectingState;
    emit stateChange(ConnectingState);
}

void QAsioTcpSocket::connectToHost(const QString & hostName, quint16 port)
{
    if (resolver_ == nullptr)
        resolver_ = new asio::ip::tcp::resolver(socket_->get_io_service());
    resolver_->async_resolve(asio::ip::tcp::resolver::query(hostName.toStdString(),QString::number(port).toStdString()),
                           std::bind(&QAsioTcpSocket::resolverHandle,this,std::placeholders::_1,std::placeholders::_2));
    state_ = ConnectingState;
    emit stateChange(ConnectingState);
}

void QAsioTcpSocket::connectedHandler(const asio::error_code &error,asio::ip::tcp::resolver::iterator iterator)
{
    if (!error) {
        state_ = ConnectedState;
        socket_->async_read_some(asio::buffer(data_,data_.max_size()),
                                 std::bind(&QAsioTcpSocket::readHandler,this,std::placeholders::_1,std::placeholders::_2));
        this->peerPoint = socket_->remote_endpoint(erro_code);
        QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::Connected,error));
    } else {
        asio::ip::tcp::resolver::iterator end;
        if(iterator == end)
        {
            state_ = UnconnectedState;
            QCoreApplication::postEvent(this,new QAsioEvent(QAsioEvent::ConnectEorro,error));
        }
    }
}

void QAsioTcpSocket::disconnectFromHost()
{
    qDebug() << "QAsioTcpSocket::disconnectFromHost()";
    if (state_ == UnconnectedState) return;
    socket_->shutdown(asio::ip::tcp::socket::shutdown_both);
}
