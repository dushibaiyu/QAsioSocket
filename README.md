# QAsioTcpSocket
把asio的tcp操作封装成类似QTCpserver的操作。

*********
基本现在已经可用。
使用方法：
    1.把QAsioSocket目录拷贝到您源码目录。
    2.在pro文件中包含include(QAsioSocket/QAsioSocket.pri)
    3.在你的项目中链接boost_system,boost_thread,boost_datetime库

    动态库加载方式正在测试。
    
**************************************
    Boost，Qt5.x版本 依赖。
        windows： vs2010 +
        linux ： gcc

    源码协议boost1.0

***************************************

        test下几个例子的说明：
        server Qt的socket模式的单线程应用。一共三个线程。2个asio的线程负责读写数据（读到用户缓存中），<br/>
            和同意新连接。一个线程（主线程）负责处理发过来的数据。
        HandleInThread 直接在asio的读取数据后回调函数里处理数据，一共三个线程。2个asio的线程负责读写<br/>
            数据和处理数据（读到数据立刻发给处理函数）。主线程空转。
        threadServer Qt的socket模式的多线程应用，一共四个线程。2个asio的线程负责读写数据（读到用户缓存中），<br/>
            和同意新连接。两个线程（其中有一个是主线程）负责处理发过来的数据。
        client 基于Qt的socket模式的客户端链接的使用。
        PingTongTest 一个简单的pingtong的客户端、、实现的很不严谨
        
        注：上面线程数目是参数，可以直接传进去指定的，上面数据是里面代码写的默认的参数。