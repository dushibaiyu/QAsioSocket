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

#include "include_asio/asio.hpp"
#include <QThread>

/// @brief asio::io_service 线程类封装 继承QThread，在单独的线程中运行一个asio的事件循环
/// @note 内部类，外部引用不到
class IOServerThread : public QThread
{
    Q_OBJECT
public:
    explicit IOServerThread(QObject *parent = 0);
    ~IOServerThread();

public:
    /// @brief 获取线程中运行的asio::io_service的引用
    asio::io_service & getIOServer(){return server;}
protected:
    void run();
private:
    asio::io_service server;
};

#endif // IOSERVERTHREAD_H
