#ifndef TESTSERVER_H
#define TESTSERVER_H

#include <QObject>
#include "qasiotcpserver.h"
#include <QTime>
#include <QDebug>

class TestServer : public QObject
{
    Q_OBJECT
public:
    explicit TestServer(QObject *parent = 0);
    ~TestServer();

signals:

public slots:
    void listen(qint16 port = 6688) {
        server.listen(port);
        qDebug() << server.maxConnections();
        startTime = QTime::currentTime();
    }
protected slots:
    void newCon(QAsioTcpSocket * socket);
    void readData();
private:
    QAsioTcpServer server;
    QTime startTime;
};

#endif // TESTSERVER_H
