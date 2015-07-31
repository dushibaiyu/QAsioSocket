#ifndef IOSERVICECLASS_H
#define IOSERVICECLASS_H

#include <vector>
#include <memory>
#include <thread>
#include <functional>
#include "include_asio/asio.hpp"

typedef asio::io_service IO_Service;
typedef asio::io_service::work IO_Service_work;

class IOServiceClass
{
public:
    explicit IOServiceClass(int size = 1) {
        work = new IO_Service_work(service);
        if (size < 1) size = 1;
        addThread(size);
    }

    ~IOServiceClass() {
        delete work;
        service.stop();
        for (std::thread * th : threadlist) {
//            th->join();
            delete th;
        }
    }
    int setThreadSize(int size = 0) {
        int csize = getThreadSize();
        if (size <= csize) {
            return csize;
        } else {
            addThread(csize - size);
            return size;
        }
    }
    int getThreadSize() const {return threadlist.size();}
    IO_Service & getService() { return service;}

protected:
    void addThread(int size){
        for (int i = 0; i < size; ++i) {
                std::thread * th = new std::thread(
                            std::bind(static_cast<std::size_t (IO_Service::*)()>(&IO_Service::run), &service));
                threadlist.push_back(th);
                th->detach();
            }
    }
private:
    IOServiceClass(const IOServiceClass &) = delete;
    IOServiceClass & operator=(const IOServiceClass & ) = delete;
    IO_Service service;
    IO_Service_work * work;
    std::vector<std::thread *> threadlist;
};

#endif // IOSERVICECLASS_H
