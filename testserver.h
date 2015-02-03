#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <QObject>
#include "qasiotcpserver.h"

class TestServer : public QObject
{
    Q_OBJECT
public:
    explicit TestServer(QObject *parent = 0);
    ~TestServer();

signals:

public slots:
    void listen(qint16 port = 6688) {server.listen(port);}
protected slots:
    void newCon(QAsioTcpSocket * socket);
    void readData();
private:
    QAsioTcpServer server;
};

#endif // TESTSERVER_H
