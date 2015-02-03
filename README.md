# QAsioTcpSocket
把asio的tcp操作封装成类似QTCpserver的操作。

*********
基本现在已经可用。
使用方法：
    1.把AsioTcp目录拷贝到您源码目录。
    2.在pro文件中包含include(AsioTcp/AsioTcp.pri)即可
    
**************************************
支持C++11的编译器，Qt5.x版本。其中mingw在release模式下崩溃。

源码协议boost1.0

现在里面包含的asio版本为1.10.2，不依赖boost，需要C++11支持