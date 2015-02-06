#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "sockets.h"
#include <QTimer>
#include <QtAlgorithms>

sockets::sockets(QObject *parent) : QObject(parent)
{
}

sockets::~sockets()
{
    if (!sos.isEmpty())
        qDeleteAll(sos);
}

void sockets::Creator(int size,int buffer)
{
    if (!sos.isEmpty()) {
        qDeleteAll(sos);
        sos.clear();
    }
    for (int i = 0; i < size; ++i) {
        sos.append(new MySocket(buffer,this));
    }
}

void sockets::start(int stime,const QString & ip,qint16 port)
{
    QTimer::singleShot(stime * 1000,this,&sockets::stop);
    for (int i = 0; i < sos.size(); ++i) {
        sos[i]->connectToHost(ip,port);
    }
}

void sockets::stop()
{
    for (int i = 0; i < sos.size(); ++i) {
        sos[i]->disconnectFromHost();
    }
    handler();
}

void sockets::handler()
{
    int revCout = 0, setCout = 0;
    qint64 revSize = 0, setSize = 0;
    for (int i = 0; i < sos.size(); ++i){
        revCout += sos.at(i)->readCount;
        revSize += sos.at(i)->readSize;
        setCout += sos.at(i)->writeCount;
        setSize += sos.at(i)->writeSize;
        emit sentData(i+1,sos.at(i)->readCount,sos.at(i)->writeCount,sos.at(i)->readSize,sos.at(i)->writeSize);
    }
    emit sentData(0,revCout,setCout,revSize,setSize);
    emit sentEnd();
}
