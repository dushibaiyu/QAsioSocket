#include "qasiowebsocketparent.h"
#include <QRegExp>
#include <QStringList>
#include "qdefaultmaskgenerator_p.h"

//static const QString ip = "(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\."
//                          "(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.(?:[01]?\\d\\d?|2[0-4]\\d|25[0-5])";


QAsioWebSocketParent::QAsioWebSocketParent(const QString &org, QObject *parent) :
    QAsioTcpSocketParent(parent),_State(UnconnectedState),
    isServer(false),m_pMaskGenerator(new QDefaultMaskGenerator),
    m_origin(org),m_mustMask(true)
{

}

QAsioWebSocketParent::~QAsioWebSocketParent()
{
    delete m_pMaskGenerator;
}

void QAsioWebSocketParent::open(const QString & url)
{
    if (_State == ConnectedState)
        disconnectFromHost();
    if (!Url::fromString(url,m_url)) {
        emit error(tr("Invalid URL."));
        return;
    }
    _State = ConnectingState;

}

bool QAsioWebSocketParent::write(const QByteArray &data)
{
    if (state() != ConnectedState)
        return false;
    writeMutex.lock();
    if (!data.isEmpty())
        writeQueue.append(data);
    if (writeQueue.isEmpty()) {
        writeMutex.unlock();
        return false;
    }
    if (writeQueue.size() == 1) {
        wirteData(writeQueue.head().data(),static_cast<std::size_t>(writeQueue.head().size()));
    }
    writeMutex.unlock();
    return true;
}

void QAsioWebSocketParent::haveErro()
{}

void QAsioWebSocketParent::hostConnected()
{
    if (!isServer) { //连接后，发送websocket协议的http请求
        m_key = generateKey();
        const QString handshake =
                createHandShakeRequest(m_url.ResourceName(),
                                       m_url.Host()
                                            % QStringLiteral(":")
                                            % QString::number(m_url.port(80)),
                                       m_origin,
                                       QString(),
                                       QString(),
                                       m_key);//新建key
        if (handshake.isEmpty()) {
            disconnectFromHost();
            emit error(tr("链接发送数据错误"));
            return;
        }
        write(handshake.toLatin1());//发送key
    }
}

void QAsioWebSocketParent::readDataed(const char * data,std::size_t bytes_transferred)
{
    qDebug() << data;
    switch (_State) {
    case ConnectingState: //TODO:处理服务器返回验证和服务器的话验证返回
    {
        if (isServer) {

        } else {

        }
    }
        break;
    case ConnectedState://TODO:数据分包处理

        break;
    default:
        break;
    }
}

bool QAsioWebSocketParent::writeDataed(std::size_t bytes_transferred)
{
    writeMutex.lock();
    if (static_cast<std::size_t>(writeQueue.head().size()) == bytes_transferred){
        writeQueue.dequeue();
        if (!writeQueue.isEmpty()) {
            wirteData(writeQueue.head().data(),static_cast<std::size_t>(writeQueue.head().size()));
        }
        writeMutex.unlock();
        return true;
    } else {
        writeMutex.unlock();
        return false;
    }
}



QByteArray QAsioWebSocketParent::generateKey() const
{
    QByteArray key;

    for (int i = 0; i < 4; ++i) {
        const quint32 tmp = m_pMaskGenerator->nextMask();
        key.append(static_cast<const char *>(static_cast<const void *>(&tmp)), sizeof(quint32));
    }

    return key.toBase64();
}

QString QAsioWebSocketParent::createHandShakeRequest(
        QString resourceName,QString host, QString origin,
        QString extensions,QString protocols,QByteArray key)
{
    QStringList handshakeRequest;
    /*if (resourceName.contains(QStringLiteral("\r\n"))) {
        error(tr("The resource name contains newlines. " \
                                      "Possible attack detected."));
        return QString();
    }
    if (host.contains(QStringLiteral("\r\n"))) {
        error(tr("The hostname contains newlines. " \
                                      "Possible attack detected."));
        return QString();
    }
    if (origin.contains(QStringLiteral("\r\n"))) {
        error(tr("The origin contains newlines. " \
                                      "Possible attack detected."));
        return QString();
    }
    if (extensions.contains(QStringLiteral("\r\n"))) {
        error(tr("The extensions attribute contains newlines. " \
                                      "Possible attack detected."));
        return QString();
    }
    if (protocols.contains(QStringLiteral("\r\n"))) {
        error(tr("The protocols attribute contains newlines. " \
                                      "Possible attack detected."));
        return QString();
    }*/
    handshakeRequest << QStringLiteral("GET ") % resourceName % QStringLiteral(" HTTP/1.1") <<
                        QStringLiteral("Host: ") % host <<
                        QStringLiteral("Upgrade: websocket") <<
                        QStringLiteral("Connection: Upgrade") <<
                        QStringLiteral("Sec-WebSocket-Key: ") % QString::fromLatin1(key);
    if (!origin.isEmpty())
        handshakeRequest << QStringLiteral("Origin: ") % origin;
    handshakeRequest << QStringLiteral("Sec-WebSocket-Version: 13");
//                            % QString::number(13);
    if (extensions.length() > 0)
        handshakeRequest << QStringLiteral("Sec-WebSocket-Extensions: ") % extensions;
    if (protocols.length() > 0)
        handshakeRequest << QStringLiteral("Sec-WebSocket-Protocol: ") % protocols;
    handshakeRequest << QStringLiteral("\r\n");

    return handshakeRequest.join(QStringLiteral("\r\n"));
}
