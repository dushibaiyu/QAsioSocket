#ifndef QASIOEVENT_H
#define QASIOEVENT_H

#include <QEvent>
#include "qasiotcpsocket.h"

class QAsioEvent : public QEvent
{
public:
    enum ConnectedEvent{
        Connected,
        DisConnect,
        ConnectEorro,
        WriteEorro,
        ReadError,
        ReadReadly
    };

    explicit QAsioEvent():QEvent(QAsioSocketEventType){}
    QAsioEvent(ConnectedEvent ctype,asio::error_code code)
        :QEvent(QAsioSocketEventType),cType_(ctype),erro_code(code){}
    static const QEvent::Type QAsioSocketEventType = (QEvent::Type)QEvent::registerEventType();

    void setSocketState(ConnectedEvent ctype){cType_ = ctype;}
    void setErrorCode(const asio::error_code & code) {erro_code = code;}

    ConnectedEvent getConnectedType() const {return cType_;}
    asio::error_code getErrorCode() const {return erro_code;}
private:
    ConnectedEvent cType_;
    asio::error_code erro_code;
};

#endif // QASIOEVENT_H
