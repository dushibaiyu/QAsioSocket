INCLUDEPATH += $$PWD/src/include_asio/
INCLUDEPATH += $$PWD/src/

win32:win32-g++:LIBS += -lMswsock
win32:LIBS += -lws2_32

DEFINES += ASIO_STANDALONE

CONFIG += c++11

HEADERS += \
    $$PWD/src/ioserviceclass.h \
    $$PWD/include/tcpabstractsocket.h \
    $$PWD/include/qasiotcpsocket.h \
    $$PWD/include/qasiotcpserver.h \
    $$PWD/src/linstenclass.hpp \
    $$PWD/src/qsocketconnection.h \
    $$PWD/include/qasiosslsocket.h \
    $$PWD/include/qasiosslserver.h \
    $$PWD/include/qasiosocket.h

SOURCES += \
    $$PWD/src/qasiotcpsocket.cpp \
    $$PWD/src/qasiotcpserver.cpp \
    $$PWD/src/qsocketconnection.cpp \
    $$PWD/src/qasiosslsocket.cpp \
    $$PWD/src/qasiosslserver.cpp
