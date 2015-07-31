# QAsioTcpSocket
把asio的socket操作封装成类似Qt Network类似的简单的api。

****************************************
###优点，目的：
		API 使用简单优雅。封装完全隐藏asio实现的细节。
		不依赖Boost。可以直接包含代码使用，也可以编译成动态库。
		
###比较Qt的socket ：
		效率高：充分利用平台特性，win（iocp），linux（epoll）。而Qt的socekt都是select。
		并发高：个人不准确测试，当QTcpServer接受900+连接后就不能在接受新的连接了。而QAsioTcpServer几千个很轻松。

**************************
###使用方法：
        1.Build:
            直接用Qt-Creator 打开项目（QAsioSocket.pro），然后打开src.pro文件.
            然后编译，生成的动态库在lib文件夹下，头文件就是include文件夹下的。
        2.直接使用源码：
            把代码复制到您的项目目录下去。
            在pro文件中包含include(QAsioSocket/QAsioSocket.pri)，
            添加宏定义 DEFINES += QASIOSOCKET_NOLIB 到您的pro文件。
    
**************************************
###环境依赖
        依赖库：Qt 5.x(Just Use Core Module), asio(Non Boost.已经在源码里包含了）
        windows： vs2013 + , mingw 4.9 +
        linux ： gcc 4.8 + <br/>

***************************************
###启用SSL：
    	SSL连接只实现了服务端，可以做ssl socket的服务器，但是没有客户端接口去连接远程SSL 服务器。
    	启用SSL 的库编译：
    	在src.pro工程文件里添加：DEFINES += QASIO_SSL  （取消哪行注视也行）
    	添加openssl的库引用。
    	直接使用源码的话，请在您的工程文件（pro）文件里加上宏QASIO_SSL。并添加openssl的库引用。

