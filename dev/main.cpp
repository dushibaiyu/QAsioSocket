
#include <QCoreApplication>
#include "qasiowebsocketparent.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QAsioWebSocketParent web;
    web.open("ws://127.0.0.1");

    return a.exec();
}
