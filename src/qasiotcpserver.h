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

#include "qasiotcpserverparent.h"

class QAsioTcpSocket;

/// @brief TcpServer的简单封装
/// @note 这是最自由化的版本，server不管理链接过来的链接，后续会增加管理的便捷类
class QASIOSOCKET_LIBRARY QAsioTcpServer : public QAsioTcpServerParent
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

signals:
    /// @brief Singanl 信号：有新连接发送此信号
    /// @param socket 新连接的指针地址
    /// @note 此版本不管理链接，新连接通过信号发送出去后，服务端不做任何处理。<br/>
    ///  也就意味着你必须链接此信号，并且注意断开连接时释放内存
    void newConnection(QAsioTcpSocket * socket);

protected:
    void incomingConnection(asio::ip::tcp::socket * socket);
    bool haveErro(const asio::error_code &);
protected:
    //自定义事件的处理，与asio事件循环线程发过来的事件的处理
    void customEvent(QEvent * e);

private:
    Q_DISABLE_COPY(QAsioTcpServer)
};

#endif // QASIOTCPSERVER_H
