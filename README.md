# QAsioTcpSocket
把asio的socket操作封装成类似Qt Network类似的简单的api。

*********
###使用方法：
        1.Build:
            直接用Qt-Creator 打开项目（QAsioSocket.pro），然后打开src.pro文件.
            然后编译，生成的动态库在lib文件夹下，头文件就是include文件夹下的。
        2.直接使用源码：（请注意协议,源码协议LGPL V2.1+）
            把代码复制到您的项目目录下去。
            在pro文件中包含include(QAsioSocket/QAsioSocket.pri)，
            添加宏定义 DEFINES += QASIOSOCKET_NOLIB 到您的pro文件。
    
**************************************
###环境依赖
        依赖库：Qt 5.x版本 
        windows： vs2013 + 
        linux ： gcc <br/>

***************************************
###test下几个例子的说明：
        server ：Qt的socket模式的单线程应用。一共三个线程。2个asio的线程负责读写数据（读到用户缓存中），
            和同意新连接。一个线程（主线程）负责处理发过来的数据。
        HandleInThread ：直接在asio的读取数据后回调函数里处理数据，一共三个线程。2个asio的线程负责读写
            数据和处理数据（读到数据立刻发给处理函数）。主线程空转。
        threadServer ：Qt的socket模式的多线程应用，一共四个线程。2个asio的线程负责读写数据（读到用户缓存中），
            和同意新连接。两个线程（其中有一个是主线程）负责处理发过来的数据。
        client ：基于Qt的socket模式的客户端链接的使用。
        PingTongTest ：一个简单的pingtong的客户端、、实现的很不严谨
        
        注：上面线程数目是参数，可以直接传进去指定的，上面数据是里面代码写的默认的参数。
