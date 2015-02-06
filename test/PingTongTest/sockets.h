#ifndef SOCKETS_H
#define SOCKETS_H

#include <QObject>
#include "mysocket.h"
#include <QVector>

class sockets : public QObject
{
    Q_OBJECT
public:
    explicit sockets(QObject *parent = 0);
    ~sockets();

    void Creator(int size, int buffer = 16384);

signals:
    void sentData(int site ,int revCou, int sentCou,qint64 revSize,qint64 sentSize);
    void sentEnd();
public slots:
    void start(int stime,const QString & ip,qint16 port);
protected slots:
    void stop();
protected:
    void handler();
private:
    QVector <MySocket *> sos;
};

#endif // SOCKETS_H
