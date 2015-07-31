#ifndef TCPABSTRACTSOCKET_H
#define TCPABSTRACTSOCKET_H

#include "qasiosocket.h"

class QSocketConnection;

class QASIOSOCKET_EXPORT TcpAbstractSocket : public QObject
{
    Q_OBJECT
public :
    enum SocketState {
        UnconnectedState = 0,
        HostFinding = 1,
        SSLHandshakeing = 3,
        ConnectingState = 2,
        ConnectedState = 4
    };

    enum SocketOption {
        Multicast_Loopback, //QAbstractSocket::MulticastLoopbackOption (bool)
        Multicast_TTL, //QAbstractSocket::MulticastTtlOption, IP_MULTICAST_TTL  (int)
        IP_TCP_NODelay, //QAbstractSocket::LowDelayOption (bool)
        Broadcast, //SO_BROADCAST (bool)
        Linger, //SO_LINGER (bool,int)
        Keep_Live,//QAbstractSocket::KeepAliveOption, SO_KEEPALIVE (bool)
        Receive_Buffer_Size, //QAbstractSocket::ReceiveBufferSizeSocketOption (int)
        Receive_Low_Watermark, //SO_RCVLOWAT (int)
        Reuse_Address, //SO_REUSEADDR (bool)
        Send_Buffer_Size,//QAbstractSocket::SendBufferSizeSocketOption (int)
        Send_Low_Watermark //SO_SNDLOWAT (int)
    };

public:
    explicit TcpAbstractSocket(QObject *parent = 0): QObject(parent)
    {}
    virtual ~TcpAbstractSocket()
    {}

signals:
    void connected();
    void disConnected();
    void stateChange(SocketState state);
    void sentReadData(const QByteArray & data);
    void erroString(const QString & erro);
    void hostFinded();
public slots:

    virtual void connectToHost(const QString &hostName, quint16 port) = 0;

    virtual void disconnectFromHost() = 0;

    inline void write(const QByteArray & data){
        sendData(data.data(),data.size());
    }

    inline void write(const char * data, int size = -1){
        sendData(data,size);
    }
    virtual void setSocketOption(TcpAbstractSocket::SocketOption option, bool isEnble, uint value) = 0;

    virtual std::pair<bool,int> getSocketOption(TcpAbstractSocket::SocketOption) = 0;


    void setSocketOption(TcpAbstractSocket::SocketOption option,bool isEnble) {
        setSocketOption(option,isEnble,0);
    }
    void setSocketOption(TcpAbstractSocket::SocketOption option,uint value) {
        setSocketOption(option,false,value);
    }

public :
    int socketDescriptor() const {return handle;}

    int errorCode() const {return erro_code;}
    QString erro() const {return erro_string;}

    SocketState state() const {return state_;}

    QString peerIp() const {return peerIp_;}
    qint16 peerPort() const  {return peerPort_; }
protected:
    virtual void sendData(const char * data, int size) = 0;

    inline void haveErro(int erro_code,const QString & erro_string) {
        this->erro_code = erro_code;
        this->erro_string = erro_string;
        setState(UnconnectedState);
        handle = -1;
        peerPort_ = 0;
        peerIp_.clear();
        haveErro();
    }
    inline void hostConnected(int handle_id, QString && ip,qint16 prot) {
        handle = handle_id;
        peerIp_ = ip;
        peerPort_ = prot;
        hostConnected();
    }
    virtual void haveErro() {
        emit erroString(erro_string);
        emit stateChange(state_);
        emit disConnected();
    }
    virtual void hostConnected() {
        setState(ConnectedState);
        emit stateChange(state_);
        emit connected();}
    virtual bool readDataed(const char * data,std::size_t bytes_transferred){
        emit sentReadData(QByteArray(data,bytes_transferred));
        return true;
    }
    virtual bool writeDataed(std::size_t /*bytes_transferred*/,std::size_t /*willWriteSize*/){
        return true;}
    virtual void finedHosted(){
        setState(ConnectingState);
        emit hostFinded();
    }
    virtual void heartTimeOut(int /*timeout*/){}

    virtual void handshaked(){}

    inline void setState(SocketState st) {this->state_ = st;}
    inline void setSocketPtr(int soc) {handle = soc;}
private :
    volatile int handle = -1;
    QString erro_string;
    int erro_code = 0;
    volatile SocketState state_ = UnconnectedState;
    QString peerIp_;
    qint16 peerPort_;

    friend class QSocketConnection;
    Q_DISABLE_COPY(TcpAbstractSocket)
};

#endif // TCPABSTRACTSOCKET_H
