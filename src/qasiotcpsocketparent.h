#ifndef QASIOTCPSOCKETPARENT_H
#define QASIOTCPSOCKETPARENT_H

#include <QObject>
#include <array>
#include <QAtomicInt>
#include <functional>
#include "ioserverthread.h"



#ifndef QASIOSOCKET_LIBRARY
#define QASIOSOCKET_LIBRARY
#endif

class QASIOSOCKET_LIBRARY QAsioTcpSocketParent : public QObject
{
    Q_OBJECT
public:
    explicit QAsioTcpSocketParent(int byteSize = 4096,QObject *parent = 0);
    QAsioTcpSocketParent(asio::ip::tcp::socket * socket ,int byteSize = 4096, QObject *parent = 0);
    virtual ~QAsioTcpSocketParent();

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
        FindHostError = 0X03,
        NoError = 0X04
    };

signals:
    /// @brief Signal 信号：链接状态改变
    /// @param state 最新的状态
    void stateChange(SocketState state);

public slots:
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
    virtual bool write(const QByteArray & data) = 0;

public:

    /// @brief 获取socket的本地描述符
    int socketDescriptor() const {
        if (socket_ == nullptr) {
            return -1;
        } else {
            return static_cast<int>(socket_->native_handle());
        }
    }

    /// @brief 获取当前连接的端点
    asio::ip::tcp::endpoint peerEndPoint() const {return this->peerPoint;}

    /// @brief 当前的链接状态
    SocketState state() const { int tp = state_; return static_cast<SocketState>(tp);}

    /// @brief 获取错误
    asio::error_code error() const {return this->erro_code;}

    SocketErroSite erroSite() const {return this->erro_site;}

#ifdef ASIO_HAS_BOOST_DATE_TIME
    void setHeartTimeOut(int s) {
        if (s <= 30) return;
        if (timer != nullptr) {
            timer->cancel();
            delete timer;
            timer = nullptr;
        }
        timeOut_s = s;
        timer = new asio::deadline_timer(socket_->get_io_service(),boost::posix_time::seconds(timeOut_s));
    }
    int getHeartTimeOut() const {return timeOut_s;}
#endif
protected:
    virtual void haveErro() = 0;
    virtual void hostConnected() = 0;
    virtual void readDataed(const char * data,std::size_t bytes_transferred) = 0;
    virtual bool writeDataed(std::size_t bytes_transferred) = 0;
    virtual void finedHosted() = 0;
    virtual void heartTimeOut(int timeout){timeout;}
protected:
    // 数据读取的回调函数
    void readHandler(const asio::error_code& error, std::size_t bytes_transferred);
    // 数据写入的回调函数
    void writeHandler(const asio::error_code& error,std::size_t bytes_transferred);
    // 链接之后的回调函数
    void connectedHandler(const asio::error_code& error, asio::ip::tcp::resolver::iterator iterator);
    // 解析主机后的回调函数
    void resolverHandle(const asio::error_code & error, asio::ip::tcp::resolver::iterator iterator);

#ifdef ASIO_HAS_BOOST_DATE_TIME
    void heartTimeOutHandler(const asio::error_code& error) {
        if (!error) {
            heartTimeOut(timeOut_s);
        }
        timer->async_wait(std::bind(&QAsioTcpSocketParent::heartTimeOutHandler,this,std::placeholders::_1));
    }
#endif

protected:
    asio::error_code erro_code;
    SocketErroSite erro_site = NoError;
    void wirteData(const char * data,std::size_t size) {
        asio::async_write(*socket_,asio::buffer(data,size),
                          std::bind(&QAsioTcpSocketParent::writeHandler,this,std::placeholders::_1,std::placeholders::_2));
    }
private:
    QAtomicInt state_ = UnconnectedState;//变成原子操作
    asio::ip::tcp::socket * socket_ = nullptr;
    asio::ip::tcp::endpoint peerPoint;
    asio::ip::tcp::resolver * resolver_ = nullptr;
#ifdef ASIO_HAS_BOOST_DATE_TIME
    asio::deadline_timer * timer = nullptr;
    int timeOut_s = 0;
#endif
    int byteSize_;
    //接受数据的缓存
//    std::array<char,4096> data_;
    char * data_ = nullptr;
    Q_DISABLE_COPY(QAsioTcpSocketParent)
};

#endif // QASIOTCPSOCKETPARENT_H
