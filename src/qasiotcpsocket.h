/// ********************************************************************************************************************
/// @file qasiotcpsocket.h
/// @brief TcpSocket的封装
/// @version 0.00.01
/// @date 2015.02.03
/// @author 渡世白玉(www.dushibaiyu.com)
///
/// ====================================================================================================================

#ifndef QASIOTCPSOCKET_H
#define QASIOTCPSOCKET_H

#include <QBuffer>
#include "ioserverthread.h"
#include <QMutex>
#include <QMutexLocker>
#include <QQueue>
#include <array>

#ifndef QASIOSOCKET_LIBRARY
#define QASIOSOCKET_LIBRARY
#endif

class QAsioTcpServer;

/// @brief TcpSocket的封装,接口按照QTcpsocket的设计的
class QASIOSOCKET_LIBRARY QAsioTcpSocket : public QObject
{
    Q_OBJECT
public:

    /// @brief 构造函数
    explicit QAsioTcpSocket(QObject *parent = 0);

    /// @brief 析构
    virtual ~QAsioTcpSocket();

    /// @brief socket的当前链接状态
    /// @note  UnconnectedState &emsp; = &emsp;0X00&emsp;&emsp;未连接 <br/>
    ///  ConnectingState &emsp;=&emsp; 0X01 &emsp;&emsp;正在连接<br/>
    ///  ConnectedState &emsp;=&emsp; 0X02 &emsp;&emsp; 已连接
    enum SocketState {
        UnconnectedState = 0X00,
        ConnectingState = 0X01,
        ConnectedState = 0X02
    };

    /// @brief socket出错的位置
    /// @note ConnectEorro &emsp;=&emsp; 0X00 &emsp;&emsp;链接过程出错<br/>
    /// ReadError &emsp;=&emsp; 0X01 &emsp;&emsp;读取错误<br/>
    /// WriteEorro &emsp;=&emsp; 0X02 &emsp;&emsp;写入错误<br/>
    /// FindHostError &emsp;=&emsp; 0X03 &emsp;&emsp;解析主机错误<br/>
    enum SocketErroSite{
        ConnectEorro = 0X00,
        ReadError = 0X01,
        WriteEorro = 0X02,
        FindHostError = 0X03
    };

Q_SIGNALS:

    /// @brief Signal 信号：数据就绪的信号
    void readReadly();

    /// @brief Signal 信号：链接成功的信号
    void connected();

    /// @brief Signal 信号：断开链接的信号
    void disconnected();

    /// @brief Signal 信号：发生错误信号
    /// @param site 发生错误的位置
    /// @param erro_code 错误码
    void sentError(const QString & site,const asio::error_code & erro_code);

    /// @brief Signal 信号：链接状态改变
    /// @param state 最新的状态
    void stateChange(SocketState state);

    /// @brief Signal 信号：解析主机成功的信号
    void hostFound();

public Q_SLOTS:

    /// @brief 链接到主机
    /// @param hostName 连接到的主机，支持域名和ip的，自动dns
    /// @param port 链接的目标端口
    void connectToHost(const QString &hostName, quint16 port);

    /// @brief 连接到主机，重载
    /// @param peerPoint 需要连接的到的asio::ip::tcp::endpoint
    void connectToHost(const asio::ip::tcp::endpoint &peerPoint);

    /// @brief 从目标主机断开链接
    void disconnectFromHost();

    /// @brief 发送数据到服务器
    /// @param data 要发送的数据，内部有队列缓存的，为空是自动执行队列
    /// @note 链接是未连接状态，和队列里没有有效数据则返回false
    bool write(const QByteArray & data = QByteArray());

public:

    /// @brief 当前可读字节数
    qint64 bytesAvailable() const {return buffer.size() - buffer.pos();}

    /// @brief 读取数据
    /// @param maxsize 最多读取多少字节
    QByteArray read(qint64 maxsize)
    {
        QMutexLocker mutexLocker(&bufferMutex);
        return buffer.read(maxsize);
    }

    /// @brief 读取全部数据
    QByteArray readAll()
    {
        QMutexLocker mutexLocker(&bufferMutex);
        return buffer.readAll();
    }

    /// @brief 读取一行数据
    QByteArray readLine()
    {
        QMutexLocker mutexLocker(&bufferMutex);
        return buffer.readLine();
    }

    /// @brief 是否读取到最后了
    bool atEnd() const{return buffer.atEnd();}

    /// @brief 获取socket的本地描述符
    int socketDescriptor() const {
        if (socket_ == nullptr) {
            return -1;
        } else {
            return static_cast<int>(socket_->native_handle());
        }
    }

    void setDisconnecdDeleteBuffer(bool isdel = false) {isDisconDelData = isdel;}

    /// @brief 获取当前连接的端点
    asio::ip::tcp::endpoint peerEndPoint() const {return this->peerPoint;}

    /// @brief 当前的链接状态
    SocketState state() const {return this->state_;}

    /// @brief 获取错误
    asio::error_code error() const {return this->erro_code;}

protected:
    // 数据读取的回调函数
    void readHandler(const asio::error_code& error, std::size_t bytes_transferred);
    // 数据写入的回调函数
    void writeHandler(const asio::error_code& error,std::size_t bytes_transferred);
    // 链接之后的回调函数
    void connectedHandler(const asio::error_code& error, asio::ip::tcp::resolver::iterator iterator);
    // 解析主机后的回调函数
    void resolverHandle(const asio::error_code & error, asio::ip::tcp::resolver::iterator iterator);

    /// @brief 直接赋给asio::ip::tcp::socket的构造函数，为保护，只支持QAsioTcpServer调用
    QAsioTcpSocket(asio::ip::tcp::socket * socket , QObject *parent = 0);//server类才能访问

    friend class QAsioTcpServer;
protected:
    // 处理自定义事件，就是asio事件循环发送过来的事件
    void customEvent(QEvent * event);

private:
    //发送的队列和队列的锁
    QMutex writeMutex;
    QQueue<QByteArray> writeQueue;

private:
    //接受数据的buffer和buffr锁
    QBuffer buffer;
    QMutex bufferMutex;
    //接受数据的缓存
    std::array<char,4096> data_;

private:
    SocketState state_;
    asio::ip::tcp::socket * socket_ = nullptr;
    asio::error_code erro_code;
    asio::ip::tcp::endpoint peerPoint;
    asio::ip::tcp::resolver * resolver_ = nullptr;
    bool isDisconDelData = false;
    Q_DISABLE_COPY(QAsioTcpSocket)
};

#endif // QASIOTCPSOCKET_H
