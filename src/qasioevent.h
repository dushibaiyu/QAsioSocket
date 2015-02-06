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
#include <QVariant>
#include "qasiotcpsocket.h"

/// @brief Socket用来线程间通讯的事件类
/// @note 内部类，外部引用不到
class QAsioEvent : public QEvent
{
public:
    enum ConnectedEvent{//socket事件代表的状态
        Connected, //已连接
        DisConnect,//断开链接
        HaveEorro,//发现错误错误
        ReadReadly, //数据可供读取
        HostFined, //找到主机
        HeartTimeOut //心跳时间到了，但是没有数据
    };

    /// @brief 默认构造函数
    explicit QAsioEvent():QEvent(QAsioSocketEventType){}

    /// @brief 构造函数，并传入需要携带的信息
    /// @param ctype 事件代表的状态
    /// @param code 如果是错误，错误信息是什么
    QAsioEvent(ConnectedEvent ctype)
        :QEvent(QAsioSocketEventType),cType_(ctype){}

    /// @brief 设置事件代表的状态
    /// @param ctype 事件代表的状态
    void setSocketState(ConnectedEvent ctype){cType_ = ctype;}
    void setData(const QVariant & data) {data_ = data;}

    /// @brief 获取事件代表的状态
    ConnectedEvent getConnectedType() const {return cType_;}

    QVariant getData() const {return data_;}
private:
    ConnectedEvent cType_;
    QVariant data_;
public:
    /// @brief 事件的类型
    static const QEvent::Type QAsioSocketEventType;

};

const QEvent::Type QAsioEvent::QAsioSocketEventType = (QEvent::Type)QEvent::registerEventType();

#endif // QASIOEVENT_H
