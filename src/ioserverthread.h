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
#include <boost/system/error_code.hpp>
#include <boost/thread.hpp>
#include <vector>

class IOServerThreadPrivate;
class QAsioTcpServerParent;

#define DEFAULT_THREAD_SIZE 2
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
    int setIoThreadSize(int size = 0);
    int getIoThreadSize() const {return threadlist.size();}
    boost::asio::io_service & getIoServer();
protected:
    explicit IOServerThread() {work = new boost::asio::io_service::work(ioserver);}
    void initThread(int size);
private:
    IOServerThread(const IOServerThread &){}
    IOServerThread & operator= (const IOServerThread & ){return *this;}
    IOServerThreadPrivate * p;
    boost::asio::io_service ioserver;
    boost::asio::io_service::work * work;
    std::vector<boost::thread *> threadlist;
};


#endif // IOSERVERTHREAD_H
