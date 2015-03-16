#ifndef QASIOWEBSOCKET_H
#define QASIOWEBSOCKET_H

#include <QObject>

class QAsioWebSocket : public QObject
{
    Q_OBJECT
public:
    explicit QAsioWebSocket(QObject *parent = 0);
    ~QAsioWebSocket();

signals:

public slots:
};

#endif // QASIOWEBSOCKET_H
