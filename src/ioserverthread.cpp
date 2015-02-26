#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "ioserverthread.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <vector>
//#include <thread>

using namespace boost::asio;

class IOServerThreadPrivate
{
public:
    ~IOServerThreadPrivate();

    int getIoThreadSize() const {return threadlist.size();}
    io_service & getIoServer() {return ioserver;}

protected:
    explicit IOServerThreadPrivate(IOServerThread * p,int size);
    friend class IOServerThread;
private:
    IOServerThread * q;
    io_service ioserver;
    io_service::work * work;
    std::vector<boost::thread *> threadlist;
};

IOServerThread::~IOServerThread()
{
    if (p)
        delete p;
}

void IOServerThread::setIoThreadSize(int size)
{
    p = new IOServerThreadPrivate(this,size);
}

int IOServerThread::getIoThreadSize() const
{
    return p->getIoThreadSize();
}

boost::asio::io_service & IOServerThread::getIoServer()
{
    if (!p) {
        p = new IOServerThreadPrivate(this,0);
    }
    return p->getIoServer();
}

IOServerThreadPrivate::IOServerThreadPrivate(IOServerThread * p, int size):
    q(p)
{
    work = new io_service::work(ioserver);
    if (size <= 0) size = 2;
    for (int i = 0; i < size; ++i) {
        boost::thread * th = new boost::thread(
                    boost::bind(&io_service::run, &ioserver));
        threadlist.push_back(th);
        th->join();
    }
}

IOServerThreadPrivate::~IOServerThreadPrivate()
{
    ioserver.stop();
    for (std::vector<boost::thread *>::size_type i = 0; i < threadlist.size(); ++i) {
        boost::thread * th = threadlist[i];
        delete th;
    }
}

