/// ********************************************************************************************************************
/// @file asioevent.h
/// @brief Socket用来线程间通讯的事件类
/// @version 0.00.01
/// @date 2015.02.03
/// @author 渡世白玉(www.dushibaiyu.com)
///
/// ====================================================================================================================

#ifndef QASIOEVENT_H
#define QASIOEVENT_H

#include <QEvent>
#include "qasiotcpsocket.h"

/// @brief Socket用来线程间通讯的事件类
/// @note 内部类，外部引用不到
class QAsioEvent : public QEvent
{
public:
    enum ConnectedEvent{//socket事件代表的状态
        Connected, //已连接
        DisConnect,//断开链接
        ConnectEorro,//链接错误
        WriteEorro,//写入错误（发送错误）
        ReadError,//读取错误
        ReadReadly,//有新数据到达并且已经读取到用户缓存
        FindHosted,//解析主机主机成功
        FindHostEorro//解析主机地址失败
    };

    /// @brief 默认构造函数
    explicit QAsioEvent():QEvent(QAsioSocketEventType){}

    /// @brief 构造函数，并传入需要携带的信息
    /// @param ctype 事件代表的状态
    /// @param code 如果是错误，错误信息是什么
    QAsioEvent(ConnectedEvent ctype,asio::error_code code)
        :QEvent(QAsioSocketEventType),cType_(ctype),erro_code(code){}

    /// @brief 设置事件代表的状态
    /// @param ctype 事件代表的状态
    void setSocketState(ConnectedEvent ctype){cType_ = ctype;}
    /// @brief 设置错误信息
    /// @param code 错误代码
    void setErrorCode(const asio::error_code & code) {erro_code = code;}

    /// @brief 获取事件代表的状态
    ConnectedEvent getConnectedType() const {return cType_;}

    /// @brief 获取事件的错误信息
    asio::error_code getErrorCode() const {return erro_code;}
private:
    ConnectedEvent cType_;
    asio::error_code erro_code;

public:
    /// @brief 事件的类型
    static const QEvent::Type QAsioSocketEventType;

};

const QEvent::Type QAsioEvent::QAsioSocketEventType = (QEvent::Type)QEvent::registerEventType();

#endif // QASIOEVENT_H
