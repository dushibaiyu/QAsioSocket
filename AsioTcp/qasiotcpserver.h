/// ********************************************************************************************************************
/// @file qasiotcpserver.h
/// @brief TcpServer的简单封装，这是最自由化的版本，server不管理链接过来的链接，后续会增加管理的便捷类
/// @version 0.00.01
/// @date 2015.02.03
/// @author 渡世白玉(www.dushibaiyu.com)
///
/// ====================================================================================================================

#ifndef QASIOTCPSERVER_H
#define QASIOTCPSERVER_H

#include <QObject>
#include <QVector>
#include "ioserverthread.h"
#include <QMutex>

class QAsioTcpSocket;
/// @brief TcpServer的简单封装
/// @note 这是最自由化的版本，server不管理链接过来的链接，后续会增加管理的便捷类
class QAsioTcpServer : public QObject
{
    Q_OBJECT
public:

    /// @brief 构造函数
    /// @param threadSize服务端开启的asio::io_service的数目
    /// @note 当threadSize<=0 时，默认是开启两个
    /// @param parent父对象指针，Qt父子关系对象的父对象
    explicit QAsioTcpServer(int threadSize = -1,QObject *parent = 0);

    /// @brief 析构函数
    virtual ~QAsioTcpServer();

    /// @brief 监听的类型枚举
    /// @note <br/>
    /// IPV4&emsp;&emsp; 只是监听IPv4 <br/>
    /// IPV6&emsp;&emsp; 只是监听IPv6 <br/>
    /// Both&emsp;&emsp; 同时监听IPv4和IPv6 <br/>
    /// None&emsp;&emsp; 不监听任何端口 <br/>
    /// TODO:在linux下测试的是IPV4和IPV6都是可以全部监听的
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

Q_SIGNALS:
    /// @brief Singanl 信号：有新连接发送此信号
    /// @param socket 新连接的指针地址
    /// @note 此版本不管理链接，新连接通过信号发送出去后，服务端不做任何处理。<br/>
    ///  也就意味着你必须链接此信号，并且注意断开连接时释放内存
    void newConnection(QAsioTcpSocket * socket);

public Q_SLOTS:

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
    //有IP4的新连接的回调
    void appectHandleV4(const asio::error_code & code);
    //有IP6的新连接的回调
    void appectHandleV6(const asio::error_code & code);

    //自定义事件的处理，与asio事件循环线程发过来的事件的处理
    void customEvent(QEvent * e);

    //切换新连接采用asio::io_service，采用公平队列，一次轮询
    inline void goForward(){lastState ++; if (lastState == threadSize_) lastState = 0;}
    //监听IP4
    inline bool linstenV4(const asio::ip::tcp::endpoint & endpoint);
    //监听IP6
    inline bool linstenV6(const asio::ip::tcp::endpoint & endpoint);
private:
    int lastState = 0,threadSize_;
    qint16 port_;
    ListenType type_ = None;
    QString ip_;
    asio::error_code error_;
    asio::ip::tcp::acceptor * apv4 = nullptr, * apv6 = nullptr;
    QVector<IOServerThread *> iosserverList;
    asio::ip::tcp::socket * socketV6 = nullptr, * socketV4 = nullptr;
};

#endif // QASIOTCPSERVER_H
