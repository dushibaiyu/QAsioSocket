/// ********************************************************************************************************************
/// @file ioserverthread.h
/// @brief asio::io_service 线程类封装
/// @version 0.00.01
/// @date 2015.02.03
/// @author 渡世白玉(www.dushibaiyu.com)
///
/// ====================================================================================================================


#ifndef IOSERVERTHREAD_H
#define IOSERVERTHREAD_H

#ifdef Q_OS_WIN
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <boost/asio.hpp>

class IOServerThreadPrivate;
class QAsioTcpServerParent;

/// @brief asio::io_service 线程类封装 继承QThread，在单独的线程中运行一个asio的事件循环
/// @note 内部类，外部引用不到
class IOServerThread
{
public:
    ~IOServerThread();
    /// @brief 获取唯一的一个后台的io_server
    static IOServerThread & getIOThread() {
        static IOServerThread server;
        return server;
    }
    void setIoThreadSize(int size = 0);
    int getIoThreadSize() const;
    boost::asio::io_service & getIoServer();
protected:
    explicit IOServerThread(): p(0) {}
    friend class QAsioTcpServerParentPrivate;
private:
    IOServerThread(const IOServerThread &){}
    IOServerThread &operator= (const IOServerThread & ){return *this;}
    IOServerThreadPrivate * p;
};


#endif // IOSERVERTHREAD_H
