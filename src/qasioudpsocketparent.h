#ifndef QASIOUDPSOCKETPARENT_H
#define QASIOUDPSOCKETPARENT_H

#include <QObject>

class QAsioUdpSocketParent : public QObject
{
    Q_OBJECT
public:
    explicit QAsioUdpSocketParent(QObject *parent = 0);
    ~QAsioUdpSocketParent();

signals:

public slots:
};

#endif // QASIOUDPSOCKETPARENT_H
