#ifndef QASIOTCPSERVERPARENT_H
#define QASIOTCPSERVERPARENT_H

#include <QObject>
#include "qasiosocket.h"

class QAsioTcpSocketParent;
class QAsioTcpServerParentPrivate;

class QASIOSOCKET_EXPORT QAsioTcpServerParent : public QObject
{
    Q_OBJECT

    friend class QAsioTcpServerParentPrivate;
public:
    explicit QAsioTcpServerParent(int threadSize, QObject *parent = 0);
    virtual ~QAsioTcpServerParent();

    /// @brief 监听的类型枚举
    /// @note <br/>
    /// IPV4&emsp;&emsp; 只是监听IPv4 <br/>
    /// IPV6&emsp;&emsp; 只是监听IPv6 <br/>
    /// Both&emsp;&emsp; 同时监听IPv4和IPv6 <br/>
    /// None&emsp;&emsp; 不监听任何端口 <br/>
    /// @warning : 在linux和MAC下始终是IPV4和IPV6同时监听的
    enum ListenType{
#ifdef Q_OS_WIN
        IPV4 = 0X04,IPV6 = 0X06,
#else
        Both = 0X01,
#endif
        None = 0X00
    };

    /// @brief 获取出错的错误信息
    int getEorrorCode() const ;

    /// @brief 当前监听的端口
    qint16 listenPort()const {return port_;}

    /// @brief 当前监听的模式
    /// @note 返回None表示当前没有在监听状态
    ListenType listenType() const {return type_;}

    /// @brief 当前监听的IP
    /// @note 返回"0"表示当前监听的是某个模式的所有IP ,<br/>
    ///  返回空表示没在监听状态
    QString listenIP() const {return ip_;}

    int getThreadSize() const {return threadSize;}


public slots:
    /// @brief 开始监听，成功返回ture，否则false
    /// @param ip 需要监听的IP地址
    /// @note 注意：现在只支持IP地址的，暂不支持域名解析，IP4或IP6皆可，需要您自己判断地址是否合法的。
    bool listen(const QString & ip,qint16 port);

    /// @brief 开始监听的重载
    /// @param port 需要监听的端口，监听当前类型的的本机所有IP
    /// @param ltype 需要监听的类型
    bool listen(qint16 port,ListenType ltype
#ifdef Q_OS_WIN
                = IPV4
#else
                = Both
#endif
            );

    /// @brief 关闭监听
    void close();

protected:
    /// @brief 监听出现错误的处理方案
    /// @param code 错误码
    /// @note 返回true表示捕捉次错误信息，接着不在监听。
    virtual bool haveErro() = 0;

    /// @brief 新连接的处理函数
    /// @param socket 新的socket链接
    /// @note 注意，此指针传下去，接着内存管理需要你负责了 <br/>
    /// 此函数在asio的线程中执行，此线程无qt的事件循环的，注意资源保护
    virtual void incomingConnection() = 0;

    bool setNewSocket(QAsioTcpSocketParent * socket);

private:
    QAsioTcpServerParentPrivate * p;
    qint16 port_;
    ListenType type_;
    QString ip_;
    int threadSize;
    Q_DISABLE_COPY(QAsioTcpServerParent)
};

#endif // QASIOTCPSERVERPARENT_H
