#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "ioserverthread.h"
#include <boost/bind.hpp>
#include <QDebug>

using namespace boost::asio;

IOServerThread::~IOServerThread()
{
    ioserver.stop();
    delete work;
    for (std::vector<boost::thread *>::size_type i = 0; i < threadlist.size(); ++i) {
        boost::thread * th = threadlist[i];
        th->interrupt();
        th->join();
        delete th;
    }
}

int IOServerThread::setIoThreadSize(int size)
{
    int tsize = threadlist.size();
    if (size > tsize ) {
        return tsize;
    } else {
        initThread(size);
        return size;
    }
}

boost::asio::io_service & IOServerThread::getIoServer()
{
    if (threadlist.empty())
        initThread(DEFAULT_THREAD_SIZE);
    return ioserver;;
}

void IOServerThread::initThread(int size)
{
    for (int i = 0; i < size; ++i) {
        boost::thread * th = new boost::thread(
                    boost::bind(&io_service::run, &ioserver));
        threadlist.push_back(th);
    }
}

