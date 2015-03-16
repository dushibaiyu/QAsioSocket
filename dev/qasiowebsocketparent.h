#ifndef QASIOWEBSOCKETPARENT_H
#define QASIOWEBSOCKETPARENT_H

#include <QObject>
#include <QMutex>
#include <QQueue>
#include "qasiotcpsocketparent.h"
#include "qmaskgenerator.h"

class QAsioWebSocketParent : public QAsioTcpSocketParent
{
    Q_OBJECT
    Q_DISABLE_COPY(QAsioWebSocketParent)
public:
    explicit QAsioWebSocketParent(const QString & org = QString(), QObject *parent = 0);
    virtual ~QAsioWebSocketParent();


    void setMaskGenerator(const QMaskGenerator *maskGenerator){
        if (maskGenerator && maskGenerator != m_pMaskGenerator) {
            delete m_pMaskGenerator;
            m_pMaskGenerator = maskGenerator;
        }
    }
    const QMaskGenerator *maskGenerator() const {return m_pMaskGenerator;}
    void setResourceName(const QString &resourceName){
        if (m_resourceName != resourceName)
            m_resourceName = resourceName;
    }
signals:
    void error(const QString & err);
public slots:
    // 打开地址，链接到地址
    void open(const QString & url);
protected:
    virtual bool write(const QByteArray &data) ;

    virtual void haveErro();
    virtual void hostConnected();
    virtual void readDataed(const char * data,std::size_t bytes_transferred);
    virtual bool writeDataed(std::size_t bytes_transferred);
    virtual void finedHosted(){}

protected:
    QByteArray generateKey() const;
    QString createHandShakeRequest(QString resourceName,QString host, QString origin,
                                   QString extensions,QString protocols,QByteArray key);
private:
    SocketState _State;
    bool isServer;
    QMutex writeMutex;
    QQueue<QByteArray> writeQueue;
private:
    QByteArray m_key;
    QMaskGenerator * m_pMaskGenerator;
    Url m_url;

    QString m_origin;
    QByteArray m_key;	//identification key used in handshake requests
    bool m_mustMask;
};

#endif // QASIOWEBSOCKETPARENT_H
