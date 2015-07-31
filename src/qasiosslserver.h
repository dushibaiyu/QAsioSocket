#ifndef QASIOSSLSERVER_H
#define QASIOSSLSERVER_H

#include "qasiosslsocket.h"
#include <QFile>
#include <tuple>

#ifdef QASIO_SSL
struct SSLSession;
struct SSLSocketSession;
struct SSLCertificate;
class IOServiceClass;

class QASIOSOCKET_EXPORT QAsioSslServer : public QObject
{
    Q_OBJECT
public:
    enum KeyType {
        ASN1,
        PEM
    };

    enum SSLOption {
        Default_Workarounds,
        No_Compression,
        No_SSLv2,
        No_SSLv3,
        No_TLSv1,
        Single_DH_Use
    };

    explicit QAsioSslServer(size_t socket_buffer = 4096,QObject *parent = 0);
    virtual ~QAsioSslServer();
signals:
    void newConnection(QAsioSslSocket * socket);

    int deleteAllLink();
public :
    inline bool setCertificateChainFile(const QString & filePath) {
        QFile file(filePath);
        if (!file.open(QFile::ReadOnly)) return false;
        setCertificateChain(file.readAll());
        file.close();
        return true;
    }
    bool setPrivateKeyFile(const QString & filePath,KeyType type) {
        QFile file(filePath);
        if (!file.open(QFile::ReadOnly)) return false;
        setPrivateKey(file.readAll(),type);
        file.close();
        return true;
    }
    bool setDHFile(const QString & filePath){
        QFile file(filePath);
        if (!file.open(QFile::ReadOnly)) return false;
        setDH(file.readAll());
        file.close();
        return true;
    }

    void setCertificateChain(const QByteArray & data);
    void setPrivateKey(const QByteArray & data,KeyType type);
    void setDH(const QByteArray & data);

    void setPassword(const QString & pass);
    void setSslOption(SSLOption option);

    bool listen(const QString & ip,qint16 port);
    bool listen(qint16 port,bool isV6 = false);

    void close();

    void setSocketOption(TcpAbstractSocket::SocketOption option, uint value) {
        setSocketOption(option,false,value);
    }
    void setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble) {
        setSocketOption(option,isEnble,0);
    }

    void setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble, uint value);

    int setThreadSize(int  size);
protected:
    virtual void incomingConnection(SSLSession * session);

    std::string sslGetPassword() const;
private:
    SSLCertificate * contx_;
    SSLSocketSession * listen_session;
    friend struct SSLSocketSession;
};

#endif //asio_ssl

#endif // QASIOSSLSERVER_H
