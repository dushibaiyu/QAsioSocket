#ifndef QASIOTCPSERVERPARENT_H
#define QASIOTCPSERVERPARENT_H

#include <QObject>
#include <QVector>
#include "ioserverthread.h"

#ifndef QASIOSOCKET_LIBRARY
#define QASIOSOCKET_LIBRARY
#endif

class QASIOSOCKET_LIBRARY QAsioTcpServerParent : public QObject
{
    Q_OBJECT
public:
    explicit QAsioTcpServerParent(int threadSize,QObject *parent = 0);
    virtual ~QAsioTcpServerParent();

    /// @brief 监听的类型枚举
    /// @note <br/>
    /// IPV4&emsp;&emsp; 只是监听IPv4 <br/>
    /// IPV6&emsp;&emsp; 只是监听IPv6 <br/>
    /// Both&emsp;&emsp; 同时监听IPv4和IPv6 <br/>
    /// None&emsp;&emsp; 不监听任何端口 <br/>
    /// @warning : 在linux下始终是IPV4和IPV6同时监听的
    enum ListenType{
        IPV4,IPV6,Both,None
    };

    /// @brief 获取出错的错误信息
    asio::error_code getEorror() const {return error_;}

    /// @brief 当前监听的端口
    qint16 listenPort()const {return port_;}

    /// @brief 当前监听的模式
    /// @note 返回None表示当前没有在监听状态
    ListenType listenType() const {return type_;}

    /// @brief 当前监听的IP
    /// @note 返回"0"表示当前监听的是某个模式的所有IP ,<br/>
    ///  返回空表示没在监听状态
    QString listenIP() const {return ip_;}

    /// @brief 当前服务端开启的asio::io_service的数目
    int getIOSize() const {return threadSize_;}


public slots:
    /// @brief 开始监听，成功返回ture，否则false
    /// @param ip 需要监听的IP地址
    /// @note 注意：现在只支持IP地址的，暂不支持域名解析，IP4或IP6皆可，需要您自己判断地址是否合法的。
    bool listen(const QString & ip,qint16 port);

    /// @brief 开始监听的重载
    /// @param port 需要监听的端口，监听当前类型的的本机所有IP
    /// @param ltype 需要监听的类型
    bool listen(qint16 port,ListenType ltype = Both);

    /// @brief 关闭监听
    void close();

protected:
    /// @brief 监听出现错误的处理方案
    /// @param code 错误码
    /// @note 返回true表示捕捉次错误信息，接着不在监听。
    virtual bool haveErro(const asio::error_code & code) = 0;

    /// @brief 新连接的处理函数
    /// @param socket 新的socket链接
    /// @note 注意，此指针传下去，接着内存管理需要你负责了
    virtual void newConnected(asio::ip::tcp::socket * socket) = 0;
protected:
    //切换新连接采用asio::io_service，采用公平队列，一次轮询
    inline void goForward(){lastState ++; if (lastState == threadSize_) lastState = 0;}
    //监听acceptor
    inline bool linstenAp(const asio::ip::tcp::endpoint & endpoint);
    //有新连接的回调
    void appectHandle(const asio::error_code & code);

private:
    int lastState = 0,threadSize_;
    qint16 port_;
    ListenType type_ = None;
    QString ip_;
    asio::error_code error_;
    asio::ip::tcp::acceptor * acceptor = nullptr;
    QVector<IOServerThread *> iosserverList;
    asio::ip::tcp::socket * socket_ = nullptr;
    Q_DISABLE_COPY(QAsioTcpServerParent)
};

#endif // QASIOTCPSERVERPARENT_H
