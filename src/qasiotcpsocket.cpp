#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

//#include "qasioevent.h"
#include "qasiotcpsocket.h"
#include <QCoreApplication>
#include <QEvent>
#include <QVariant>
#include <QDebug>

class QAsioEvent : public QEvent
{
public:
    enum ConnectedEvent{//socket事件代表的状态
        Connected, //已连接
        DisConnect,//断开链接
        HaveEorro,//发现错误错误
        ReadReadly, //数据可供读取
        HostFined, //找到主机
        HeartTimeOut //心跳时间到了，但是没有数据
    };

    /// @brief 默认构造函数
    explicit QAsioEvent():QEvent(QAsioSocketEventType){}

    /// @brief 构造函数，并传入需要携带的信息
    /// @param ctype 事件代表的状态
    /// @param code 如果是错误，错误信息是什么
    QAsioEvent(ConnectedEvent ctype)
        :QEvent(QAsioSocketEventType),cType_(ctype){}

    /// @brief 设置事件代表的状态
    /// @param ctype 事件代表的状态
    void setSocketState(ConnectedEvent ctype){cType_ = ctype;}
    void setData(const QVariant & data) {data_ = data;}

    /// @brief 获取事件代表的状态
    ConnectedEvent getConnectedType() const {return cType_;}

    QVariant getData() const {return data_;}
private:
    ConnectedEvent cType_;
    QVariant data_;
public:
    /// @brief 事件的类型
    static const QEvent::Type QAsioSocketEventType;

};

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

